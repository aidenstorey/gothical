// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Inventory/Inventory.h"
#include "../../Inventory/InventorySlot.h"

//This Include
#include "SkillConsume.h"

// Implementation
void USkillConsume::Initialise()
{
	this->ReceiveInitialise();
}

bool USkillConsume::Activate()
{
	bool bReturn{ true };

	if ( !this->ReceiveActivate() )
	{
		bReturn = false;
	}
	else
	{
		auto pInventory = this->m_pOwningInventorySlot->GetOwningInventory();
		if ( pInventory != nullptr )
		{
			pInventory->RemoveSkill( this, false );
		}
	}

	return ( bReturn );
}