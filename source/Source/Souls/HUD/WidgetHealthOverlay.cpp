// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure std::min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes
#include <algorithm>

// Local Includes

//This Include
#include "WidgetHealthOverlay.h"

// Implementation
float UWidgetHealthOverlay::GetDesired() const
{
	return ( this->m_fDesired );
}

void UWidgetHealthOverlay::SetDesired( const float& _krfDesired )
{
	this->m_fDesired = _krfDesired;
}

float UWidgetHealthOverlay::GetCurrent() const
{
	return ( this->m_fCurrent );
}

void UWidgetHealthOverlay::SetCurrent( const float& _krfCurrent )
{
	this->m_fCurrent = _krfCurrent;
}

ESlateVisibility UWidgetHealthOverlay::GetOverlayVisibility() const
{
	return ( this->m_fLeft == this->m_fRight ? ESlateVisibility::Hidden : ESlateVisibility::Visible );
}

float UWidgetHealthOverlay::GetBarLeft() const
{
	return ( this->m_fLeft );
}

float UWidgetHealthOverlay::GetBarRight() const
{
	return ( this->m_fRight );
}

float UWidgetHealthOverlay::GetBarRightPercent() const
{
	return ( this->m_fRightPercent );
}

void UWidgetHealthOverlay::NativeTick( const FGeometry& _krtGeometry, float _fDeltaTime )
{
	UUserWidget::NativeTick( _krtGeometry, _fDeltaTime );
	if ( this->m_fCurrent != this->m_fDesired )
	{
		bool bLessThan{ this->m_fCurrent < this->m_fDesired };
		this->m_fCurrent = bLessThan ? 
			std::min( this->m_fCurrent + _fDeltaTime * m_fTimeScale, this->m_fDesired ) : 
			std::max( this->m_fCurrent - _fDeltaTime * m_fTimeScale, this->m_fDesired );

		float fX{ _krtGeometry.GetLocalSize().X };
		float fMin{ bLessThan ? this->m_fCurrent : this->m_fDesired };
		float fMax{ bLessThan ? this->m_fDesired : this->m_fCurrent };

		this->m_fLeft = fX * fMin;
		this->m_fRight = fX * fMax;
		this->m_fRightPercent = ( this->m_fRight - this->m_fLeft ) / fX;
	}
}
