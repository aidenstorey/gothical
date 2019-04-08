// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "WidgetBase.h"

//This Include
#include "WidgetMenuButton.h"

// Implementation
TEnumAsByte< EMenuButtonState > UWidgetMenuButton::GetButtonState() const
{
	return ( this->m_eButtonState );
}

void UWidgetMenuButton::SetButtonState( TEnumAsByte< EMenuButtonState > _eState )
{
	this->m_eButtonState = _eState;
}

ESlateVisibility UWidgetMenuButton::GetNormalVisibility() const
{
	return ( this->m_eButtonState == EMenuButtonState::BS_NORMAL ? ESlateVisibility::Visible : ESlateVisibility::Hidden );
}

ESlateVisibility UWidgetMenuButton::GetHoverVisibility() const
{
	return ( this->m_eButtonState == EMenuButtonState::BS_HOVER ? ESlateVisibility::Visible : ESlateVisibility::Hidden );
}

ESlateVisibility UWidgetMenuButton::GetClickedVisibility() const
{
	return ( this->m_eButtonState == EMenuButtonState::BS_CLICKED ? ESlateVisibility::Visible : ESlateVisibility::Hidden );
}

UTexture2D* UWidgetMenuButton::GetNormalImage() const
{
	return ( this->m_pNormalImage );
}

UTexture2D* UWidgetMenuButton::GetHoverImage() const
{
	return ( this->m_pHoverImage );
}

UTexture2D* UWidgetMenuButton::GetClickedImage() const
{
	return ( this->m_pClickedImage );
}

UWidgetBase* UWidgetMenuButton::GetOwningWidget() const
{
	return ( this->m_pOwningWidget );
}

void UWidgetMenuButton::SetOwningWidget( UWidgetBase* _pWidget )
{
	this->m_pOwningWidget = _pWidget;
}

void UWidgetMenuButton::MouseFocus()
{
	if ( this->m_pOwningWidget != nullptr )
	{
		this->m_pOwningWidget->MenuSelectedMouse( this->m_iMenuIndex );
	}
}

void UWidgetMenuButton::MouseUnfocus()
{
	if ( this->m_pOwningWidget != nullptr )
	{
		this->m_pOwningWidget->MenuDeselectMouse();
	}
}