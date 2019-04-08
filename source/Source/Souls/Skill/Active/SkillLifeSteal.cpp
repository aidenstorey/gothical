// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Enemy/Characters/EnemyCharacter.h"
#include "../../Player/Character/CharacterPlayer.h"
#include "../../Player/Controller/ControllerPlayer.h"

//This Include
#include "SkillLifeSteal.h"

// Implementation
USkillLifeSteal::USkillLifeSteal()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillLifeSteal'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillLifeSteal::ParticleComplete()
{
	this->m_pOwningController->PlayerHeal( this->m_iHealAmount );
}

void USkillLifeSteal::ReceiveInitialise()
{
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Initial" };
		auto pData = this->m_pSkillStatsDataTable->FindRow< FSkillLifeStealData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_fDamageMultiplier	= pData->m_fDamageMultiplier;
			this->m_fRadius				= pData->m_fRadius;
			this->m_iMaxTargets			= pData->m_iMaxTargets;
		}
	}
	
	this->m_tDisplayName = FText::FromString( "LifeSteal" );
}

bool USkillLifeSteal::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if it is possible to activate skill
		// True - update the anim state of controller 
	if ( this->m_pOwningController != nullptr && 
		 this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_LIFESTEAL );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillLifeSteal::ReceiveTrigger()
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
			FVector vPlayerPosition{ pPlayer->GetActorLocation() };
			float fDamageAmount{ static_cast< float >( this->m_pOwningController->GetCharacteristics()->GetDamage() ) * this->m_fDamageMultiplier };
			this->m_iHealAmount = static_cast< int32 >( fDamageAmount );

			// Perform a sphere collision to detect enemies
			TArray< FOverlapResult > arrOverlapList;
			pPlayer->GetWorld()->OverlapMultiByObjectType( arrOverlapList, vPlayerPosition, FQuat::Identity,
														   FCollisionObjectQueryParams::AllDynamicObjects,// ECC_TO_BITFIELD( ECC_GameTraceChannel3 ) | ECC_TO_BITFIELD( ECC_GameTraceChannel6 ), // Enemy Trace Channel
														   FCollisionShape::MakeSphere( this->m_fRadius ),
														   FCollisionQueryParams{ TEXT( "LifeStealOverlap" ), false, pPlayer } );



			// Find Targets up to max, calculating health gained
			int32 iHealthRegen{ 0 };
			for ( int iTarget{ 0 }; iTarget < arrOverlapList.Num() && iTarget < this->m_iMaxTargets; iTarget++ )
			{
				// Check if target is an enemy character
					// True - apply lifesteal effect and start particles
				auto pTarget = Cast< AEnemyCharacter >( arrOverlapList[ iTarget ].GetActor() );
				if ( pTarget != nullptr )
				{
					// Check if there is a trigger particle attached
						// True - spawn particle between target and player
					if ( this->m_pTriggerParticles != nullptr )
					{
						auto pParticle = UGameplayStatics::SpawnEmitterAtLocation( pPlayer->GetWorld(),
																				   this->m_pTriggerParticles,
																				   vPlayerPosition );

						pParticle->SetBeamSourcePoint( 0, pTarget->GetActorLocation(), 0 );
						pParticle->SetBeamTargetPoint( 0, vPlayerPosition, 0 );
					}

					// Apply damage
					UGameplayStatics::ApplyDamage( pTarget, fDamageAmount, this->m_pOwningController, pPlayer, UDamageType::StaticClass() );
					iHealthRegen += static_cast< int32 >( fDamageAmount );
				}
			}

			if ( iHealthRegen > 0 )
			{
				this->m_pOwningController->PlayerHeal( iHealthRegen );
			}

			// Set detected flag and updated the cooldown time remaining
			this->m_fTimeRemaining = this->m_fCooldown;
			this->m_bTriggered = true;

			bReturn = true;
		}
	}

	return ( bReturn );
}