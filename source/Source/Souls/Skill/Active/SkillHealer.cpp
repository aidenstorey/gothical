// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Player/Controller/ControllerPlayer.h"
#include "../../Player/Animation/AnimInstancePlayer.h"

//This Include
#include "SkillHealer.h"

// Implementation
USkillHealer::USkillHealer()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillHealer'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillHealer::ReceiveInitialise()
{
	// Check if there is a valid data table 
		// True - initialise fields with data from table
	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Default" };
		auto pData = this->m_pSkillStatsDataTable->FindRow< FSkillHealerData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_fDuration = pData->m_fDuration;
			this->m_fRate = pData->m_fRate;
		}
	}

	this->m_tDisplayName = FText::FromString( "Healer" );
}

bool USkillHealer::ReceiveActivate()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller and if it is possible to activate skill
		// True - update the anim state of controller and spawn particles
	if ( this->m_pOwningController != nullptr && 
		 this->m_pOwningController->GetCharacteristics()->GetHealthPercent() < 1.0f && 
		 this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_HEALER );

		// Check if there are particles for activate
			// True - run the particles 
		if ( this->m_pActivateParticles )
		{
			auto pMeshComponent = this->m_pOwningController->GetAnimInstancePlayer()->GetSkelMeshComponent();
			UGameplayStatics::SpawnEmitterAttached( this->m_pActivateParticles,
													pMeshComponent,
													FName( "ground_socket" ) );
		}

		bReturn = true;
	}

	return ( bReturn );
}

bool USkillHealer::ReceiveTrigger()
{
	bool bReturn{ false };

	// Check if there is a valid owning controller
		// True - set player to regen
	if ( this->m_pOwningController != nullptr )
	{
		this->m_pOwningController->PlayerRegen( this->m_fDuration, this->m_fRate );

		// Set detected flag and updated the cooldown time remaining
		this->m_fTimeRemaining = this->m_fCooldown;
		this->m_bTriggered = true;

		bReturn = true;
	}

	return ( bReturn );
}