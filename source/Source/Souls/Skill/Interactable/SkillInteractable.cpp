// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Inventory/Inventory.h"
#include "../SkillBase.h"

//This Include
#include "SkillInteractable.h"

// Implementation
ASkillInteractable::ASkillInteractable( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	this->m_pInteractArea = _krtInitialiser.CreateDefaultSubobject< UCapsuleComponent >( this, "InteractArea" );
	this->RootComponent = this->m_pInteractArea;
	this->m_pInteractArea->SetCollisionProfileName( FName( "Interactable" ) );
}

UCapsuleComponent* ASkillInteractable::GetInteractArea() const
{
	return ( this->m_pInteractArea );
}

USkillBase* ASkillInteractable::GetSkill() const
{
	return ( this->m_pSkill );
}

bool ASkillInteractable::Initialise( USkillBase* _pSkill )
{
	bool bInitialised{ false };

	if ( _pSkill != nullptr )
	{
		this->m_pSkill = _pSkill;

		this->m_pInteractArea->SetCapsuleSize( this->m_fInteractRadius, this->m_fInteractHalfHeight );

		bInitialised = this->ReceiveInitialise( _pSkill );
	}

	return ( bInitialised );
}

bool ASkillInteractable::Interact( UInventory* _pInventory )
{
	bool bSuccessful{ false };

	if ( _pInventory != nullptr && this->m_pSkill != nullptr )
	{
		bSuccessful = this->ReceiveInteract( _pInventory );
	}

	return ( bSuccessful );
}

bool ASkillInteractable::ReceiveInitialise( USkillBase* _pSkill )
{
	bool bReturn{ true };

	// ... 

	return ( bReturn );
}

bool ASkillInteractable::ReceiveInteract( UInventory* _pInventory )
{
	bool bReturn{ true };

	// ...

	return ( bReturn );
}