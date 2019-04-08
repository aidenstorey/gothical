// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Skill/SkillBase.h"
#include "../Skill/Active/SkillActive.h"
#include "../Skill/Passive/SkillPassive.h"

//This Include
#include "WidgetHotbarBadge.h"

// Implementation
TEnumAsByte< EInputMethod > UWidgetHotbarBadge::GetInputMehtod()
{
	return ( this->m_eInputMethod );
}

void UWidgetHotbarBadge::SetInputMethod( TEnumAsByte< EInputMethod > _eMethod )
{
	this->m_eInputMethod = _eMethod;
}

bool UWidgetHotbarBadge::GetShowInventory()
{
	return ( this->m_bShowInventory );
}

void UWidgetHotbarBadge::SetShowInventory( bool _bShow )
{
	this->m_bShowInventory = _bShow;
}

USkillBase* UWidgetHotbarBadge::GetSkill()
{
	return ( this->m_pSkill );
}

void UWidgetHotbarBadge::SetSkill( class USkillBase* _pSkill )
{
	this->m_pSkill = _pSkill;
}

UTexture2D* UWidgetHotbarBadge::GetHealthIcon()
{
	return ( this->m_pHealthIcon );
}

UTexture2D* UWidgetHotbarBadge::GetDamageIcon()
{
	return ( this->m_pDamageIcon );
}

UTexture2D* UWidgetHotbarBadge::GetAttackSpeedIcon()
{
	return ( this->m_pAttackSpeedIcon );
}

UTexture2D* UWidgetHotbarBadge::GetMovementSpeedIcon()
{
	return ( this->m_pMovementSpeedIcon );
}

UTexture2D* UWidgetHotbarBadge::GetActivateButtonIcon()
{
	return ( this->m_eInputMethod == EInputMethod::IM_CONTROLLER ? this->m_pActivateButtonIconController : this->m_pActivateButtonIconKeyboard );
}

ESlateVisibility UWidgetHotbarBadge::DetermineHealthIconVisibility()
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	// Check if skill is passive and has appropriate modifier
		// True - set visibility return to visible
	auto pPassive = Cast< USkillPassive >( this->m_pSkill );
	if ( pPassive != nullptr && pPassive->HasHealthModifier() && !this->m_bShowInventory )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}

ESlateVisibility UWidgetHotbarBadge::DetermineDamageIconVisibility()
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	// Check if skill is passive and has appropriate modifier
		// True - set visibility return to visible
	auto pPassive = Cast< USkillPassive >( this->m_pSkill );
	if ( pPassive != nullptr && pPassive->HasDamageModifier() && !this->m_bShowInventory )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}

ESlateVisibility UWidgetHotbarBadge::DetermineAttackSpeedIconVisibility()
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	// Check if skill is passive and has appropriate modifier
		// True - set visibility return to visible
	auto pPassive = Cast< USkillPassive >( this->m_pSkill );
	if ( pPassive != nullptr && pPassive->HasAttackSpeedModifier() && !this->m_bShowInventory )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}

ESlateVisibility UWidgetHotbarBadge::DetermineMovementSpeedIconVisibility()
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	// Check if skill is passive and has appropriate modifier
		// True - set visibility return to visible
	auto pPassive = Cast< USkillPassive >( this->m_pSkill );
	if ( pPassive != nullptr && pPassive->HasMovementSpeedModifier() && !this->m_bShowInventory )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}

ESlateVisibility UWidgetHotbarBadge::DetermineActivateIconVisibility()
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	// Check if skill is active or if in inventory
		// True - set visibility return to visible
	if ( this->m_bShowInventory )
	{
		eReturn = ESlateVisibility::Visible;
	}
	else if ( this->m_pSkill != nullptr && this->m_pSkill->GetType() == USkillBase::ESkillType::ST_ACTIVE )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}