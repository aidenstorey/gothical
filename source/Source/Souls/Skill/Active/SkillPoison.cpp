// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

//This Include
#include "SkillPoison.h"

// Implementation
USkillPoison::USkillPoison()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillPoison'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillPoison::ReceiveInitialise()
{
	// Check if there is a valid data table 
		// True - initialise fields with data from table
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Initial" };
		auto pData = this->m_pSkillStatsDataTable->FindRow< FSkillPoisonData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_fTrailDuration			= pData->m_fTrailDuration;
			this->m_fTrailSpawnRate			= pData->m_fTrailSpawnRate;
			this->m_pTrailSpawn				= pData->m_pTrailSpawn;
			this->m_fEffectLifetime			= pData->m_fEffectLifetime;
			this->m_fEffectRadius			= pData->m_fEffectRadius;
			this->m_fPoisonDuration			= pData->m_fPoisonDuration;
			this->m_fPoisonRate				= pData->m_fPoisonRate;
			this->m_fPoisonDamageMultiplier = pData->m_fPoisonDamageMultiplier;
		}
	}

	this->m_tDisplayName = FText::FromString( "Poison" );
}

bool USkillPoison::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if skill can be activated
		// True - update the anim state of controller
	if ( this->m_pOwningController != nullptr && this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_POISON );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillPoison::ReceiveTrigger()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller
		// True - begin the poison trail
	if ( this->m_pOwningController != nullptr )
	{
		this->m_pOwningController->BeginPoisonTrail( this->m_fTrailDuration,
													 this->m_fTrailSpawnRate,
													 this->m_pTrailSpawn,
													 this->m_fEffectLifetime,
													 this->m_fEffectRadius,
													 this->m_fPoisonDuration,
													 this->m_fPoisonRate,
													 static_cast< int32 >( this->m_pOwningController->GetCharacteristics()->GetDamage() * static_cast< float >( this->m_fPoisonDamageMultiplier ) ) );

		// Set detected flag and updated the cooldown time remaining
		this->m_fTimeRemaining = this->m_fCooldown;
		this->m_bTriggered = true;

		bReturn = true;
	}

	return ( bReturn );
}