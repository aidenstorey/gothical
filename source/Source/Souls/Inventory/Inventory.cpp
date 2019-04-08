// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../HUD/WidgetBase.h"
#include "../Player/Controller/ControllerPlayer.h"
#include "../Skill/SkillBase.h"
#include "InventorySlot.h"

//This Include
#include "Inventory.h"

// Implementation
int32 UInventory::GetHotBarSlotCount() const
{
	return ( this->m_iHotBarSlotCount );
}

int32 UInventory::GetStorageSlotCount() const
{
	return ( this->m_iStorageSlotCount );
}

UInventorySlot* UInventory::GetSlot( const int32& _kriSlotNumber ) const
{
	UInventorySlot* pReturn{ nullptr };

	if ( _kriSlotNumber >= 0 && _kriSlotNumber < this->m_arrSlots.Num() )
	{
		pReturn = this->m_arrSlots[ _kriSlotNumber ];
	}


	return ( pReturn );
}

USkillBase* UInventory::GetSlotContents( const int32& _kriSlotNumber ) const
{
	USkillBase* pReturn{ nullptr };

	UInventorySlot* pSlot{ this->GetSlot( _kriSlotNumber ) };
	if ( pSlot != nullptr )
	{
		pReturn = pSlot->GetSkill();
	}

	return ( pReturn );
}

AControllerPlayer* UInventory::GetOwningController() const
{
	return ( this->m_pOwningController );
}

UCharacteristics* UInventory::GetOwningCharacteristics() const
{
	return ( this->m_pOwningCharacteristics );
}

UAnimInstancePlayer* UInventory::GetOwningAnimInstance() const
{
	return ( this->m_pOwningAnimInstance );
}

FInventoryDropDelegate& UInventory::GetOnDropSkillDelegate()
{
	return ( this->m_tOnDropSkill );
}

bool UInventory::IsSlotOccupied( const int32& _kriSlotNumber ) const
{
	bool bReturn{ true };

	UInventorySlot* pSlot{ this->GetSlot( _kriSlotNumber ) };
	if ( pSlot == nullptr )
	{
		bReturn = false;
	}
	else
	{
		bReturn = !pSlot->IsUnoccupied();
	}


	return ( bReturn );
}

bool UInventory::CanAddSkill() const
{
	return ( this->FirstUnoccupiedSlotNumber() != this->m_arrSlots.Num() );
}

int UInventory::FirstUnoccupiedSlotNumber() const
{
	int32 iSlot{ 0 };
	for ( ; iSlot < this->m_arrSlots.Num() && !this->m_arrSlots[ iSlot ]->IsUnoccupied(); iSlot++ );

	return ( iSlot );
}

void UInventory::Initialise( const int& _kriHotBarSlotCount, const int& _kriStorageSlotCount, class AControllerPlayer* _pController, class UCharacteristics* _pCharacteristics, class UAnimInstancePlayer* _pAnimInstance )
{
	this->m_iHotBarSlotCount = _kriHotBarSlotCount;
	this->m_iStorageSlotCount = _kriStorageSlotCount;
	this->m_pOwningController = _pController;
	this->m_pOwningCharacteristics = _pCharacteristics;
	this->m_pOwningAnimInstance = _pAnimInstance;

	this->m_arrSlots.Empty();

	for ( int iHotBar{ 0 }; iHotBar < this->m_iHotBarSlotCount; iHotBar++ )
	{
		FString strName{ "HotBar" + FString::FromInt( iHotBar ) };
		UInventorySlot* pSlot{ NewObject< UInventorySlot >( this, FName( *strName ) ) };
		pSlot->SetOwningInventory( this );
		pSlot->SetType( UInventorySlot::ESlotType::ST_HOTBAR );
		this->m_arrSlots.Add( pSlot );
	}

	for ( int iStorage{ 0 }; iStorage < this->m_iStorageSlotCount; iStorage++ )
	{
		FString strName{ "Storage" + FString::FromInt( iStorage ) };
		UInventorySlot* pSlot{ NewObject< UInventorySlot >( this, FName( *strName ) ) };
		pSlot->SetOwningInventory( this );
		pSlot->SetType( UInventorySlot::ESlotType::ST_STORAGE );
		this->m_arrSlots.Add( pSlot );
	}
}

bool UInventory::AddSkill( USkillBase* _pSkill )
{
	bool bReturn{ true };

	int32 iSlot{ this->FirstUnoccupiedSlotNumber() };
	if ( iSlot == this->m_arrSlots.Num() )
	{
		bReturn = false;
	}
	else
	{
		_pSkill->SetOwningController( this->m_pOwningController );
		bReturn = this->m_arrSlots[ iSlot ]->AddSkill( _pSkill );
	}

	return ( bReturn );
}

bool UInventory::ForceAddSkill( const int32& _kriSlotNumber, USkillBase* _pSkill )
{
	bool bReturn{ true };

	UInventorySlot* pSlot{ this->GetSlot( _kriSlotNumber ) };
	if ( pSlot == nullptr )
	{
		bReturn = false;
	}
	else
	{
		USkillBase* pRemovedSkill{ pSlot->RemoveSkill() };
		if ( pRemovedSkill != nullptr )
		{
			pRemovedSkill->SetOwningController( nullptr );
		}

		_pSkill->SetOwningController( this->m_pOwningController );
		bReturn = pSlot->AddSkill( _pSkill );
	}

	return ( bReturn );
}

bool UInventory::SwapSkill( const int32& _kriSlotNumberA, const int32& _kriSlotNumberB )
{
	bool bReturn{ false };

	auto pSlotA = this->GetSlot( _kriSlotNumberA );
	auto pSlotB = this->GetSlot( _kriSlotNumberB );

	if ( pSlotA != nullptr && pSlotB != nullptr && pSlotA != pSlotB )
	{
		bReturn = pSlotA->SwapSkill( pSlotB );
	}

	return ( bReturn );
}

bool UInventory::RemoveSkill( USkillBase* _pSkill, const bool _bDrop )
{
	bool bRemoved{ false };

	if ( _pSkill != nullptr )
	{
		for ( int iSlot{ 0 }; iSlot < this->m_arrSlots.Num() && !bRemoved; iSlot++ )
		{
			if ( this->m_arrSlots[ iSlot ]->HasSkill( _pSkill ) )
			{
				USkillBase* pSkill{ this->m_arrSlots[ iSlot ]->RemoveSkill() };
				pSkill->SetOwningController( nullptr );

				bRemoved = true;

				if ( _bDrop )
				{
					this->m_tOnDropSkill.Broadcast( pSkill );
				}
			}
		}
	}

	return ( bRemoved );
}

bool UInventory::RemoveFromSlot( const int32& _kriSlotNumber, const bool _bDrop )
{
	bool bRemoved{ false };

	UInventorySlot* pSlot{ this->GetSlot( _kriSlotNumber ) };
	if ( pSlot != nullptr )
	{
		if ( pSlot->GetSkill() != nullptr && !pSlot->GetSkill()->OnCooldown() )
		{
			USkillBase* pSkill{ pSlot->RemoveSkill() };
			if ( pSkill != nullptr )
			{
				pSkill->SetOwningController( nullptr );
				bRemoved = true;

				if ( _bDrop )
				{
					this->m_tOnDropSkill.Broadcast( pSkill );
				}
			}
		}
	}

	return ( bRemoved );
}

void UInventory::RemoveAll( const bool _bDrop )
{
	for ( auto aSlot : this->m_arrSlots )
	{
		USkillBase* pSkill{ aSlot->RemoveSkill() };
		if ( pSkill != nullptr )
		{
			pSkill->SetOwningController( nullptr );

			if ( _bDrop )
			{
				this->m_tOnDropSkill.Broadcast( pSkill );
			}
		}
	}
}

bool UInventory::AttachWidgetIcons( TArray< class UWidgetInventoryIcon* >& _rarrIcons )
{
	bool bReturn{ false };

	if ( this->m_arrSlots.Num() == _rarrIcons.Num() )
	{
		bReturn = true;

		for ( int iSlot{ 0 }; iSlot < this->m_arrSlots.Num() && bReturn; iSlot++ )
		{
			this->m_arrSlots[ iSlot ]->SetIcon( _rarrIcons[ iSlot ] );
		}
	}

	return ( bReturn );
}

bool UInventory::Process( const float& _krfDeltaTime )
{
	bool bReturn{ true };

	for ( auto pSlot : this->m_arrSlots )
	{
		pSlot->Process( _krfDeltaTime );
	}
	
	return ( bReturn );
}