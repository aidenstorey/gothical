// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Player/Controller/ControllerPlayer.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/InventorySlot.h"
#include "../Player/Animation/AnimInstancePlayer.h"
#include "SkillData.h"

//This Include
#include "SkillBase.h"

// Implementation
USkillBase::USkillBase()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/Skills'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillDataTable = pSkillData.Object;
	}
}

USkillBase::ESkillType USkillBase::GetType() const
{
	return ( this->m_eType );
}

UTexture2D* USkillBase::GetTexture() const
{
	return ( this->m_pTexture );
}

FText USkillBase::GetToolTip() const
{
	return ( this->m_tToolTip );
}

FText USkillBase::GetDisplayName() const
{
	return ( this->m_tDisplayName );
}

UClass* USkillBase::GetSkillDrop() const
{
	return ( this->m_pSkillDrop );
}

AControllerPlayer* USkillBase::GetOwningController() const
{
	return ( this->m_pOwningController );
}

void USkillBase::SetOwningController( AControllerPlayer* _pController )
{
	this->m_pOwningController = _pController;
}

void USkillBase::SetActivated( bool _bState )
{
	this->m_bActivated = _bState;
}

bool USkillBase::IsActivated() const
{
	return ( this->m_bActivated );
}

bool USkillBase::OnCooldown() const
{
	return ( false );
}

bool USkillBase::HasCastStun() const
{
	return ( this->m_bCastStun );
}

float USkillBase::GetCooldownPercentage() const
{
	return ( 0.0f );
}

int32 USkillBase::PassiveIncreaseCount() const
{
	return ( 0 );
}

bool USkillBase::SetOwningInventorySlot( class UInventorySlot* _pInventorySlot )
{
	bool bReturn{ true };

	if ( this != nullptr )
	{
		if ( this->m_eType == ESkillType::ST_PASSIVE )
		{
			bool bIsCurrentHotBar = this->m_pOwningInventorySlot != nullptr && this->m_pOwningInventorySlot->IsHotBar();
			bool bIsNewHotBar = _pInventorySlot != nullptr && _pInventorySlot->IsHotBar();

			if ( bIsCurrentHotBar && !bIsNewHotBar )
			{
				bReturn = this->Deactivate();
			}
			else if ( !bIsCurrentHotBar && bIsNewHotBar )
			{
				this->Activate();
			}
		}
		else if ( _pInventorySlot != nullptr )
		{
			this->m_pOwningController = _pInventorySlot->GetOwningInventory()->GetOwningController();
		}

		if ( bReturn )
		{
			this->m_pOwningInventorySlot = _pInventorySlot;
		}
	}

	return ( bReturn );
}

void USkillBase::Initialise()
{
	this->ReceiveInitialise();
	this->LoadSkillInfo();
}

void USkillBase::LoadSkillInfo()
{
	if ( this->m_pSkillDataTable != nullptr )
	{
		FString tName{ this->m_tDisplayName.ToString() };
		FSkillData* pData{ this->m_pSkillDataTable->FindRow< FSkillData >( FName( *tName ), TEXT( "LookUpOperation" ) ) };
		if ( pData != nullptr )
		{
			if ( !pData->m_tToolTip.IsEmptyOrWhitespace() )
			{
				this->m_tToolTip = pData->m_tToolTip;
			}

			this->m_fCooldown = pData->m_fCooldownTime;
			this->m_bCastStun = pData->m_bCastStun;
			this->m_pTexture = pData->m_pIcon;
			this->m_pSkillDrop = pData->m_pSkillDrop;
			this->m_pActivateParticles = pData->m_pActivateParticles;
			this->m_pDeactivateParticles = pData->m_pDeactivateParticles;
			this->m_pTriggerParticles = pData->m_pTriggerParticles;
			this->m_pSoundCueActivate = pData->m_pSoundCueActivate;
			this->m_pSoundCueRecurring = pData->m_pSoundCueRecurring;
		}
	}
}

bool USkillBase::Activate()
{
	bool bReturn{ false };

	if ( !this->OnCooldown() && !this->m_bActivated && this->ReceiveActivate() )
	{
		this->m_bActivated = true;
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBase::Deactivate()
{
	bool bReturn{ false };

	if ( this->m_bActivated && this->ReceiveDeactivate() )
	{
		this->m_bActivated = false;
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBase::Trigger()
{
	bool bReturn{ false };

	if ( this->ReceiveTrigger() )
	{
		if ( this->m_pSoundCueActivate != nullptr )
		{
			UGameplayStatics::PlaySoundAtLocation( this->m_pOwningController->GetWorld(), this->m_pSoundCueActivate, this->m_pOwningController->GetCharacter()->GetActorLocation() );
		}
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBase::Interupt()
{
	bool bReturn{ false };

	if ( this->m_bActivated && this->ReceiveInterupt() )
	{
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBase::Process( const float& _krfDeltaTime )
{
	return ( this->ReceiveProcess( _krfDeltaTime ) );
}

void USkillBase::ReceiveInitialise()
{
	// ...
}

bool USkillBase::ReceiveActivate()
{
	bool bReturn{ true };

	//..

	return ( bReturn );
}

bool USkillBase::ReceiveDeactivate()
{
	bool bReturn{ true };

	//..

	return ( bReturn );
}

bool USkillBase::ReceiveTrigger()
{
	bool bReturn{ true };

	//..

	return ( bReturn );
}

bool USkillBase::ReceiveInterupt()
{
	bool bReturn{ true };

	this->m_bActivated = false;

	return ( bReturn );
}

bool USkillBase::ReceiveProcess( const float& _krfDeltatime )
{
	bool bReturn{ true };

	// ...

	return ( bReturn );
}