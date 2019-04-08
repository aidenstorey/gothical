// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Skill/SkillBase.h"
#include "WidgetHotbarBadge.h"

//This Include
#include "WidgetInventoryIcon.h"

// Implementation
bool UWidgetInventoryIcon::GetSelected() const
{
	return ( this->m_bSelected );
}

void UWidgetInventoryIcon::SetSelected( bool _bSelected )
{
	this->m_bSelected = _bSelected;
}

USkillBase* UWidgetInventoryIcon::GetSkill() const
{
	return ( this->m_pSkill );
}

void UWidgetInventoryIcon::SetSkill( USkillBase* _pSkill )
{
	this->m_pSkill = _pSkill;
	if ( this->m_pBadge != nullptr )
	{
		this->m_pBadge->SetSkill( _pSkill );
	}
}

void UWidgetInventoryIcon::SetBadge( class UWidgetHotbarBadge* _pBadge )
{
	this->m_pBadge = _pBadge;
}

ESlateVisibility UWidgetInventoryIcon::GetSelectedEmptyVisibility() const
{
	ESlateVisibility eVisibility{ ESlateVisibility::Hidden };

	if ( this->m_pSkill == nullptr && this->m_bSelected )
	{
		eVisibility = ESlateVisibility::Visible;
	}

	return ( eVisibility );
}

ESlateVisibility UWidgetInventoryIcon::GetSelectedIconVisibility() const
{
	ESlateVisibility eVisibility{ ESlateVisibility::Hidden };

	if ( this->m_pSkill != nullptr && this->m_bSelected )
	{
		eVisibility = ESlateVisibility::Visible;
	}

	return ( eVisibility );
}

UTexture2D* UWidgetInventoryIcon::GetSkillTexture() const
{
	UTexture2D* pTexture{ nullptr };

	if ( this->m_pSkill != nullptr )
	{
		pTexture = this->m_pSkill->GetTexture();
	}

	return ( pTexture );
}

float UWidgetInventoryIcon::GetSkillCooldownPercentage() const
{
	float fPercentage{ 0.0f };

	if ( this->m_pSkill != nullptr )
	{
		fPercentage = this->m_pSkill->GetCooldownPercentage();
	}

	return ( fPercentage );
}

FLinearColor UWidgetInventoryIcon::GetColorAndOpacity() const
{
	FLinearColor vColor{ 1.0f, 1.0f, 1.0f, 0.0f };

	if ( this->m_pSkill != nullptr )
	{
		vColor.A = 1.0f;
	}

	return ( vColor );
}

UWidgetBase* UWidgetInventoryIcon::GetOwningWidget() const
{
	return ( this->m_pOwningWidget );
}

void UWidgetInventoryIcon::SetOwningWidget( UWidgetBase* _pWidget )
{
	this->m_pOwningWidget = _pWidget;
}

void UWidgetInventoryIcon::MouseFocus()
{
	if ( this->m_pOwningWidget != nullptr && this->m_pOwningWidget->GetShowInventory() )
	{
		this->m_pOwningWidget->SetSelected( this->m_iRowPosition, this->m_iColPosition );
	}
}