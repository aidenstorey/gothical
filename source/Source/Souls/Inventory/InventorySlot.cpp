// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../HUD/WidgetInventoryIcon.h"
#include "../Skill/SkillBase.h"

//This Include
#include "InventorySlot.h"

// Implementation
UInventorySlot::ESlotType UInventorySlot::GetType() const
{
	return ( this->m_eType );
}

void UInventorySlot::SetType( const ESlotType& _kreType )
{
	this->m_eType = _kreType;
}

UInventorySlot::ESlotState UInventorySlot::GetState() const
{
	return ( this->m_eState );
}

USkillBase* UInventorySlot::GetSkill() const
{
	return ( this->m_pSkill );
}

void UInventorySlot::SetSkill( class USkillBase* _pSkill )
{
	this->m_pSkill = _pSkill;
	this->m_eState = this->m_pSkill == nullptr ? ESlotState::SS_UNOCCUPIED : ESlotState::SS_OCCUPIED;
	this->m_pIcon->SetSkill( this->m_pSkill );
}

UInventory* UInventorySlot::GetOwningInventory() const
{
	return ( this->m_pOwningInventory );
}

void UInventorySlot::SetOwningInventory( UInventory* _pInventory )
{
	this->m_pOwningInventory = _pInventory;
}

UWidgetInventoryIcon* UInventorySlot::GetIcon() const
{
	return ( this->m_pIcon );
}

void UInventorySlot::SetIcon( UWidgetInventoryIcon* _pIcon )
{
	this->m_pIcon = _pIcon;
}

bool UInventorySlot::IsHotBar() const
{
	return ( this->m_eType == ESlotType::ST_HOTBAR );
}

bool UInventorySlot::IsUnoccupied() const
{
	return ( this->m_eState == ESlotState::SS_UNOCCUPIED );
}

bool UInventorySlot::HasSkill( class USkillBase* _pSkill ) const
{
	return ( this->m_pSkill == _pSkill );
}

bool UInventorySlot::AddSkill( class USkillBase* _pSkill )
{
	bool bReturn{ false };
	
	if ( this->m_eState != ESlotState::SS_OCCUPIED && _pSkill != nullptr )
	{
		if ( _pSkill->SetOwningInventorySlot( this ) )
		{
			this->SetSkill( _pSkill );

			bReturn = true;
		}
	}

	return ( bReturn );
}

USkillBase* UInventorySlot::ReplaceSkill( USkillBase* _pSkill )
{
	USkillBase* pReturn{ nullptr };

	bool bWasOccupied{ this->m_pSkill != nullptr };
	auto pRemoved = this->RemoveSkill();

	if ( bWasOccupied && pRemoved != nullptr )
	{
		this->AddSkill( _pSkill );
		pReturn = pRemoved;
	}

	return ( pReturn );
}

USkillBase* UInventorySlot::RemoveSkill()
{
	USkillBase* pReturn{ nullptr };

	if ( this->m_eState == ESlotState::SS_OCCUPIED )
	{
		if ( this->m_pSkill->SetOwningInventorySlot( nullptr ) )
		{
			pReturn = this->m_pSkill;
			this->SetSkill( nullptr );
		}
	}

	return ( pReturn );
}

bool UInventorySlot::SwapSkill( UInventorySlot* _pOtherSlot )
{
	bool bReturn{ false };

	bool bHasSkillA = !this->IsUnoccupied();
	bool bHasSkillB = !_pOtherSlot->IsUnoccupied();

	bool bOnCoolDownA = bHasSkillA && this->m_pSkill->OnCooldown();
	bool bOnCoolDownB = bHasSkillB && _pOtherSlot->GetSkill()->OnCooldown();

	if ( !bOnCoolDownA && !bOnCoolDownB )
	{
		auto pSkillA = this->m_pSkill;
		auto pSkillB = _pOtherSlot->GetSkill();
		
		bool bCanSwapA{ pSkillA->SetOwningInventorySlot( _pOtherSlot ) };
		bool bCanSwapB{ pSkillB->SetOwningInventorySlot( this ) };

		if ( bCanSwapA && bCanSwapB )
		{
			this->SetSkill( pSkillB );
			_pOtherSlot->SetSkill( pSkillA );
		}
		else
		{
			pSkillA->SetOwningInventorySlot( this );
			pSkillB->SetOwningInventorySlot( _pOtherSlot );
		}

		bReturn = bCanSwapA && bCanSwapB;
	}

	return ( bReturn );
}

bool UInventorySlot::Process( const float& _krfDeltaTime )
{
	bool bReturn{ false };

	if ( this->m_pSkill != nullptr )
	{
		bReturn = this->m_pSkill->Process( _krfDeltaTime );
	}

	return ( bReturn );
}