// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include <algorithm>

// Library Includes

// Local Includes
#include "../../Attribute/Attribute.h"
#include "../../Attribute/Characteristics.h"
#include "../../HUD/WidgetBase.h"
#include "../../Inventory/Inventory.h"
#include "../../Skill/Active/SkillActive.h"
#include "../../Skill/Active/SkillBasicAttack.h"
#include "../../Skill/Interactable/SkillDrop.h"
#include "../../Skill/Interactable/SkillInteractable.h"
#include "../../Skill/SkillBase.h"
#include "../../Skill/SkillManager.h"
#include "../../SoulsCheatManager.h"
#include "../../SoulsGameInstance.h"
#include "../../SoulsGameMode.h"
#include "../Animation/AnimInstancePlayer.h"
#include "../Character/CharacterPlayer.h"

//This Include
#include "ControllerPlayer.h"

// Implementation
void AControllerPlayer::PlayerSetHealth( int32 _iAmount )
{
	this->m_pCharacteristics->GetHealthAttribute()->SetValue( _iAmount );
	this->m_pWidgetInstance->SetHealth( static_cast< float >( this->m_pCharacteristics->GetHealthAttribute()->GetValue() ) /
										static_cast< float >( this->m_pCharacteristics->GetHealthAttribute()->GetMax() ) );
}

void AControllerPlayer::PlayerSetHealthMax( int32 _iAmount )
{
	this->m_pCharacteristics->GetHealthAttribute()->SetMax( _iAmount );
}

void AControllerPlayer::PlayerSetDamage( int32 _iAmount )
{
	this->m_pCharacteristics->GetDamageAttribute()->SetValue( _iAmount );
}

void AControllerPlayer::PlayerSetAttackSpeed( int32 _iAmount )
{
	this->m_pCharacteristics->GetAttackSpeedAttribute()->SetValue( _iAmount );
}

void AControllerPlayer::PlayerSetMovementSpeed( int32 _iAmount )
{
	this->m_pCharacteristics->GetAttackSpeedAttribute()->SetValue( _iAmount );
}

void AControllerPlayer::PlayerFullHeal()
{
	this->m_pCharacteristics->Heal();
}

void AControllerPlayer::PlayerHeal( int32 _iAmount )
{
	this->m_pCharacteristics->Heal( _iAmount );
}
void AControllerPlayer::PlayerRegen( float _fTime, float _fRate )
{
	this->m_pCharacteristics->Regen( _fTime, _fRate );
}

void AControllerPlayer::PlayerInvincibilityActivate()
{
	this->m_pCharacteristics->ActivateInvincibility();
}

void AControllerPlayer::PlayerInvincibilityDeactivate()
{
	this->m_pCharacteristics->DeactivateInvincibility();
}

void AControllerPlayer::PlayerKill()
{
	this->m_pCharacteristics->Kill();
}

void AControllerPlayer::PlayerTakeDamage( int32 _iAmount )
{
	this->m_pCharacteristics->TakeDamage( _iAmount );
}

void AControllerPlayer::PlayerInventoryAdd( int32 _iType )
{
	USoulsGameInstance* pInstance{ Cast< USoulsGameInstance >( this->GetGameInstance() ) };
	if ( pInstance != nullptr )
	{
		USkillBase* pSkill{ pInstance->GetSkillManager()->CreateSkill( static_cast< ESkillClassType >( _iType ) ) };
		if ( pSkill != nullptr )
		{
			this->m_pInventory->AddSkill( pSkill );
		}
	}
}

void AControllerPlayer::PlayerInventoryAddSlot( int32 _iSlotNumber, int32 _iType )
{
	auto pInstance = Cast< USoulsGameInstance >( this->GetGameInstance() );
	if ( pInstance != nullptr )
	{
		auto pSkill = pInstance->GetSkillManager()->CreateSkill( static_cast< ESkillClassType >( _iType ) );
		if ( pSkill != nullptr )
		{
			this->m_pInventory->ForceAddSkill( _iSlotNumber, pSkill );
		}
	}
}

void AControllerPlayer::PlayerInventoryAddCustomPassive( int32 _iHealth, int32 _iDamage, int32 _iAttackSpeed, int32 _iMomementSpeed )
{
	auto pInstance = Cast< USoulsGameInstance >( this->GetGameInstance() );
	if ( pInstance != nullptr )
	{
		auto pSkill = pInstance->GetSkillManager()->CreateCustomPassive( _iHealth, _iDamage, _iAttackSpeed, _iMomementSpeed );
		if ( pSkill != nullptr )
		{
			this->m_pInventory->AddSkill( pSkill );
		}
	}

}

void AControllerPlayer::PlayerInventoryFill( int32 _iType )
{
	auto pInstance = Cast< USoulsGameInstance >( this->GetGameInstance() );
	if ( pInstance != nullptr )
	{
		ESkillClassType eType{ static_cast< ESkillClassType >( _iType ) };
		if( pInstance->GetSkillManager()->IsSkillAvailable( eType ) )
		{
			while( this->m_pInventory->CanAddSkill() )
			{
				this->m_pInventory->AddSkill( pInstance->GetSkillManager()->CreateSkill( eType ) );
			}
		}
	}
}

void AControllerPlayer::PlayerInventoryFillRandom()
{
	auto pInstance = Cast< USoulsGameInstance >( this->GetGameInstance() );
	if( pInstance != nullptr )
	{
		while( this->m_pInventory->CanAddSkill() )
		{
			this->m_pInventory->AddSkill( pInstance->GetSkillManager()->CreateRandomSkill() );
		}
	}
}

void AControllerPlayer::PlayerInventoryDrop( int32 _iSlotNumber )
{
	this->m_pInventory->RemoveFromSlot( _iSlotNumber );
}

void AControllerPlayer::PlayerInventoryDropAll()
{
	this->m_pInventory->RemoveAll();
}

void AControllerPlayer::PlayerInventoryRemove( int32 _iSlotNumber )
{
	this->m_pInventory->RemoveFromSlot( _iSlotNumber, false );
}

void AControllerPlayer::PlayerInventoryRemoveAll()
{
	this->m_pInventory->RemoveAll( false );
}

void AControllerPlayer::PlayerInventorySwap( int32 _iSlotNumberA, int32 _iSlotNumberB )
{
	this->m_pInventory->SwapSkill( _iSlotNumberA, _iSlotNumberB );
}

void AControllerPlayer::PlayerInputSetMethod( int32 _iMethod )
{
	this->SetInputMethod( static_cast< EInputMethod >( _iMethod ) );
}

void AControllerPlayer::PlayerWinConditionToggleDisabled()
{
	this->m_bWinScreenDisabled = !this->m_bWinScreenDisabled;

	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		pGameMode->SetWinConditionDisabled( this->m_bWinScreenDisabled );
	}

	this->SaveConfig();
}

void AControllerPlayer::PlayerWinConditionAdd( int32 _iCondition )
{
	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		pGameMode->AddCondition( static_cast< EWinCondition >( 1 << _iCondition ) );
	}
}

void AControllerPlayer::PlayerWinConditionRemove( int32 _iCondition )
{
	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		pGameMode->RemoveCondition( static_cast< EWinCondition >( 1 << _iCondition ) );
	}
}

void AControllerPlayer::PlayerMovementTeleportSnap( float _fX, float _fY )
{
	this->Teleport( FVector{ _fX, _fY, this->GetCharacter()->GetActorLocation().Z }, true );
}

void AControllerPlayer::PlayerMovementTeleport( float _fX, float _fY, float _fZ )
{
	this->Teleport( FVector{ _fX, _fY, _fZ }, false );
}

void AControllerPlayer::PlayerMovementTeleportRelativeSnap( float _fX, float _fY )
{
	this->TeleportRelative( FVector{ _fX, _fY, this->GetCharacter()->GetActorLocation().Z }, true );
}

void AControllerPlayer::PlayerMovementTeleportRelative( float _fX, float _fY, float _fZ )
{
	this->TeleportRelative( FVector{ _fX, _fY, _fZ }, false );
}