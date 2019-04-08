// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

// This Include
#include "WidgetInteractableOverlay.h"

// Static Variable
FString UWidgetInteractableOverlay::m_tButtonName;

// Implementation
FVector UWidgetInteractableOverlay::GetWorldPosition() const
{
	return ( this->m_vWorldPosition );
}

FString UWidgetInteractableOverlay::GetText() const
{
	return ( "Press " + this->m_tButtonName + this->m_tText );
}

void UWidgetInteractableOverlay::SetOwner( class APlayerController* _pOwner )
{
	this->m_pOwner = _pOwner;
}

void UWidgetInteractableOverlay::Initialise( FVector _vWorldPosition, FString _tText )
{
	this->m_vWorldPosition = _vWorldPosition;
	this->m_tText = _tText;

	this->AddToViewport();
	this->SetVisibility( ESlateVisibility::Hidden );
}

void UWidgetInteractableOverlay::UpdatePosition()
{
	if ( this->m_pOwner != nullptr )
	{
		FVector2D vScreenPosition;
		this->m_pOwner->ProjectWorldLocationToScreen( this->m_vWorldPosition, vScreenPosition );

		this->SetPositionInViewport( vScreenPosition );
	}
}

void UWidgetInteractableOverlay::SetButtonName( FString _tButtonName )
{
	m_tButtonName = _tButtonName;
}