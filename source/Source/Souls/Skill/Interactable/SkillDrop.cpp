// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Inventory/Inventory.h"

//This Include
#include "SkillDrop.h"

// Implementation
ASkillDrop::ASkillDrop( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	this->m_fInteractRadius = 40.0f;
	this->m_fInteractHalfHeight = 80.0f;
	this->m_tHoverString = " to pickup";
}

bool ASkillDrop::ReceiveInteract( class UInventory* _pInventory )
{
	bool bSuccessful{ false };

	if ( _pInventory->AddSkill( this->m_pSkill ) )
	{
		bSuccessful = true;
		this->Destroy();

		// TODO:	Determine if any animation etc needs to occur
	}

	return ( bSuccessful );
}