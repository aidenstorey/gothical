// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Effect/SkillEffectPushback.h"

//This Include
#include "SkillPushback.h"

// Implementation
USkillPushback::USkillPushback()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillPushback'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillPushback::ReceiveInitialise()
{
	// Check if there is a valid data table 
		// True - initialise fields with data from table
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Initial" };
		auto pData = this->m_pSkillStatsDataTable->FindRow< FSkillPushbackData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_pPushbackEffect = pData->m_pPushbackEffect;
			this->m_fEffectDuration	= pData->m_fEffectDuration;
			this->m_fEffectRadius	= pData->m_fEffectRadius;

		}
	}

	this->m_tDisplayName = FText::FromString( "Pushback" );
}

bool USkillPushback::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if skill can be activated
		// True - update the anim state of controller
	if ( this->m_pOwningController != nullptr && this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_PUSHBACK );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillPushback::ReceiveTrigger()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller
		// True - spawn pushback effect
	if ( this->m_pOwningController != nullptr )
	{
		// Store out effect transform
		FVector vSpawnLocation{ this->m_pOwningController->GetCharacter()->GetMesh()->GetSocketLocation( FName( "ground_socket" ) ) };
		auto tTransform = FTransform{ FRotator::ZeroRotator, vSpawnLocation };

		// Check if spawning of effect worked
			// True - initialise effect
		auto pPushback = Cast< ASkillEffectPushback >( UGameplayStatics::BeginSpawningActorFromClass(
			this->m_pOwningController->GetWorld(),
			this->m_pPushbackEffect,
			tTransform ) );
		if ( pPushback != nullptr )
		{
			// Intialise effect and finish spawning
			pPushback->Initialise( this->m_fEffectDuration, this->m_fEffectRadius );
			UGameplayStatics::FinishSpawningActor( pPushback, tTransform );

			// Set detected flag and updated the cooldown time remaining
			this->m_fTimeRemaining = this->m_fCooldown;
			this->m_bTriggered = true;

			bReturn = true;
		}
	}

	return ( bReturn );
}