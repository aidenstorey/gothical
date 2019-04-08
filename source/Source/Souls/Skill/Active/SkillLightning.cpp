// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Enemy/Characters/EnemyCharacter.h"

//This Include
#include "SkillLightning.h"

// Implementation
USkillLightning::USkillLightning()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillLightning'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillLightning::ReceiveInitialise()
{
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Initial" };
		auto pData = this->m_pSkillStatsDataTable->FindRow< FSkillLightningData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_fDamageMultiplier	= pData->m_fDamageMultiplier;
			this->m_fDamageFallOff		= pData->m_fDamageFallOff;
			this->m_fInitialDistance	= pData->m_fInitialDistance;
			this->m_fArcDelay			= pData->m_fArcDelay;
			this->m_fArcDistance		= pData->m_fArcDistance;
			this->m_iMaxChain			= pData->m_iMaxChain;
			this->m_pArcParticles		= pData->m_pArcParticles;
			this->m_pHitParticles		= pData->m_pHitParticles;
			this->m_fCleanupDelay		= pData->m_fCleanupDelay;

		}
	}

	this->m_tDisplayName = FText::FromString( "Lightning" );
}

bool USkillLightning::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if it is possible to activate skill
		// True - update the anim state of controller 
	if ( this->m_pOwningController != nullptr && 
		 !this->m_pOwningController->IsLightningActive() &&
		 this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_LIGHTNING );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillLightning::ReceiveTrigger()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller
		// True - attempt to cast attack
	if ( this->m_pOwningController != nullptr )
	{
		// Check if controller has a character
			// True - cast attack damaging all enemies within range and activate screen shake
		auto pPlayer = Cast< ACharacterPlayer >( this->m_pOwningController->GetCharacter() );
		if ( pPlayer != nullptr )
		{
			// Get Relevant player data
			FVector vPlayerLocation{ pPlayer->GetActorLocation() };
			FVector vPlayerForward{ pPlayer->GetActorLocation() };

			// Covnert angle to radians
			float fAngleRadians{ ( this->m_fInitialAngle * PI ) / 180.0f };

			// Perform a sphere collision to detect enemies
			TArray< FOverlapResult > arrOverlapList;
			pPlayer->GetWorld()->OverlapMultiByObjectType( arrOverlapList, vPlayerLocation, FQuat::Identity,
														   FCollisionObjectQueryParams::AllDynamicObjects,//ECC_TO_BITFIELD( ECC_GameTraceChannel3 ) | ECC_TO_BITFIELD( ECC_GameTraceChannel6 ),								// Enemy Trace Channel
														   FCollisionShape::MakeSphere( this->m_fInitialDistance ),
														   FCollisionQueryParams{ TEXT( "LightningOverlap" ), false, pPlayer } );

			// Find closest target within FOV
			float fClosestDistance{ MAX_FLT };
			AEnemyCharacter* pClosestTarget{ nullptr };
			for ( auto& rOverlap : arrOverlapList )
			{
				// Check if overlapped actor is an enemy character
					// True - Determine if it is able to be hit
				auto pTarget = Cast< AEnemyCharacter >( rOverlap.GetActor() );
				if ( pTarget != nullptr )
				{
					// Check if target is within angle of forward vector and closer than current
						// True - update closest target
					FVector vTargetDirection{ pTarget->GetActorLocation() - vPlayerLocation };
					float fDistance{ vTargetDirection.Size() };
					if ( acos( FVector::DotProduct( vPlayerForward, vTargetDirection ) ) < fAngleRadians && fDistance < fClosestDistance )
					{
						fClosestDistance = fDistance;
						pClosestTarget = pTarget;
					}
				}
			}

			// Check if a target has been found
				// True - begin lightning on the player controller
			if ( pClosestTarget != nullptr )
			{
				this->m_pOwningController->BeginLightning( pClosestTarget,
														   this->m_fDamageMultiplier,
														   this->m_fDamageFallOff,
														   this->m_fArcDelay,
														   this->m_fArcDistance,
														   this->m_iMaxChain,
														   this->m_pArcParticles,
														   this->m_pHitParticles,
														   this->m_fCleanupDelay );
			}

			// Set detected flag and updated the cooldown time remaining
			this->m_fTimeRemaining = this->m_fCooldown;
			this->m_bTriggered = true;

			bReturn = true;
		}	
	}

	return ( bReturn );
}