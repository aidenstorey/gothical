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
#include "SkillEarthquake.h"

// Implementation
USkillEarthquake::USkillEarthquake()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillEarthquake'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillEarthquake::ReceiveInitialise()
{
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Initial" };
		FSkillEarthquakeData* pData{ this->m_pSkillStatsDataTable->FindRow< FSkillEarthquakeData >( FName( *tName ), TEXT( "LookUpOperation" ) ) };
		if ( pData != nullptr )
		{
			this->m_pCameraShake = pData->m_pCameraShake;
			this->m_iDamageMultiplier = pData->m_iDamageMultiplier;
			this->m_iRadius = pData->m_iRadius;
			this->m_iMaxTargets = pData->m_iMaxTargets;
			this->m_pDecal = pData->m_pDecal;
		}
	}

	this->m_tDisplayName = FText::FromString( "Earthquake" );
}

bool USkillEarthquake::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if it is possible to activate skill
		// True - update the anim state of controller 
	if ( this->m_pOwningController != nullptr && this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_EARTHQUAKE );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillEarthquake::ReceiveTrigger()
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
			// Perform a sphere collision to detect enemies
			TArray< FOverlapResult > arrOverlapList;
			pPlayer->GetWorld()->OverlapMultiByObjectType( arrOverlapList, pPlayer->GetActorLocation(), FQuat::Identity,
														   FCollisionObjectQueryParams::InitType::AllDynamicObjects,
														   FCollisionShape::MakeSphere( static_cast< float >( this->m_iRadius ) ),
														   FCollisionQueryParams{ TEXT( "PushbackOverlap" ), false, pPlayer } );

			// Initialise required damage variables
			FDamageEvent tDamageEvent;
			tDamageEvent.DamageTypeClass = UDamageType::StaticClass();
			int32 iDamageAmount{ this->m_pOwningController->GetCharacteristics()->GetDamage() * this->m_iDamageMultiplier };

			// Activate screenshake
			this->m_pOwningController->ClientPlayCameraShake( this->m_pCameraShake, 40.0f, ECameraAnimPlaySpace::Type::CameraLocal, FRotator::ZeroRotator );

			// Iterate over possible hit enemies
			for ( int32 iIndex{ 0 }, iTargetCount{ 0 }; iIndex < arrOverlapList.Num() && iTargetCount < this->m_iMaxTargets; iIndex++ )
			{
				// Check if target is an enemy
					// True - apply the modified damage amount 
				auto pEnemy = Cast< AEnemyCharacter >( arrOverlapList[ iIndex ].GetActor() );
				if ( pEnemy != nullptr )
				{
					pEnemy->TakeDamage( static_cast< float >( iDamageAmount ), tDamageEvent, this->m_pOwningController, pPlayer );

					FVector vSpawnLocation{ pEnemy->GetMesh()->GetSocketLocation( FName( "ground_socket" ) ) };
					UGameplayStatics::SpawnEmitterAtLocation( pEnemy->GetWorld(),
															  this->m_pActivateParticles,
															  vSpawnLocation,
															  pEnemy->GetActorRotation() );

					iTargetCount++;
				}
			}

			// Check if there are particles for activate
				// True - attempt to spawn particle emitter
			if ( this->m_pActivateParticles != nullptr )
			{
				// Check if there is a controlled character
					// True - spawn particle emitter
				auto pCharacter = this->m_pOwningController->GetCharacter();
				if ( pCharacter != nullptr )
				{
					FVector vSpawnLocation{ pCharacter->GetMesh()->GetSocketLocation( FName( "ground_socket" ) ) };
					UGameplayStatics::SpawnEmitterAtLocation( this->m_pOwningController->GetWorld(),
															  this->m_pActivateParticles,
															  vSpawnLocation,
															  pCharacter->GetActorRotation() );

					auto pDecal = UGameplayStatics::SpawnDecalAtLocation( this->m_pOwningController->GetWorld(),
															this->m_pDecal,
															FVector{ static_cast< float >( this->m_iRadius ) * 2.0f, static_cast< float >( this->m_iRadius ) * 2.0f, 500.0f },
															pCharacter->GetActorLocation(),
															FVector{ 0.0f, 0.0f, 90.0f }.Rotation(),
															1.0f );
				}
			}

			// Set detected flag and updated the cooldown time remaining
			this->m_fTimeRemaining = this->m_fCooldown;
			this->m_bTriggered = true;

			bReturn = true;
		}
	}

	return ( bReturn );
}