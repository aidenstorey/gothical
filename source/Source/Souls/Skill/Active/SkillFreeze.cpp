// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Effect/SkillEffectFreeze.h"

//This Include
#include "SkillFreeze.h"

// Implementation
USkillFreeze::USkillFreeze()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillFreeze'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillFreeze::ReceiveInitialise()
{
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Initial" };
		auto pData = this->m_pSkillStatsDataTable->FindRow< FSkillFreezeData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_pEffect				= pData->m_pEffect;
			this->m_fStunDuration		= pData->m_fStunDuration;
			this->m_fEffectDuration		= pData->m_fEffectDuration;
			this->m_fEffectSpeed		= pData->m_fEffectSpeed;
			this->m_fEffectAngle		= pData->m_fEffectAngle;
		}
	}

	this->m_tDisplayName = FText::FromString( "Freeze" );
}

bool USkillFreeze::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if it is possible to activate skill
		// True - update the anim state of controller 
	if ( this->m_pOwningController != nullptr &&
		 !this->m_pOwningController->IsLightningActive() &&
		 this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_FREEZE );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillFreeze::ReceiveTrigger()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller
		// True - spawn freeze effect
	if ( this->m_pOwningController != nullptr )
	{
		// Check if there is a controlled character
			// True - spawn particle emitter
		auto pCharacter = this->m_pOwningController->GetCharacter();
		if ( pCharacter != nullptr )
		{
			// Store out effect transform
			FVector vSpawnLocation{ pCharacter->GetMesh()->GetSocketLocation( FName( "ground_socket" ) ) };
			auto tTransform = FTransform{ pCharacter->GetActorRotation(), pCharacter->GetActorLocation() };

			// Check if spawning of effect worked
				// True - initialise effect
			auto pFreeze = Cast< ASkillEffectFreeze >( UGameplayStatics::BeginSpawningActorFromClass(
				this->m_pOwningController->GetWorld(),
				this->m_pEffect,
				tTransform ) );
			if ( pFreeze != nullptr )
			{
				// Intialise effect and finish spawning
				UGameplayStatics::FinishSpawningActor( pFreeze, tTransform );
				pFreeze->Initialise( this->m_fStunDuration, this->m_fEffectDuration, this->m_fEffectSpeed, this->m_fEffectAngle );

				// Check if there are particles for activate
					// True - attempt to spawn particle emitter
				if ( this->m_pTriggerParticles != nullptr )
				{

					UGameplayStatics::SpawnEmitterAtLocation( this->m_pOwningController->GetWorld(),
															  this->m_pTriggerParticles,
															  vSpawnLocation,
															  pCharacter->GetActorRotation() );
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