// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes
#include <algorithm>

// Local Includes
#include "AttributeModifier.h"

//This Include
#include "Attribute.h"

// Implementation
UAttribute::UAttribute() :
	m_strName( "Attribute" ),
	m_eType( EAttributeType::ET_STATIC ),
	m_iValueMin( 0 ),
	m_iValueMax( 0 ),
	m_iValueModifiedMax( 0 ),
	m_iValue( 0 )
{
	// ...
}

void UAttribute::Initialise( const FString& _krstrName, const EAttributeType& _kreType, const int32& _kriMin, const int32& _kriMax, const int32& _kriValue )
{
	// Store local copy of parameters
	this->m_strName = _krstrName;
	this->m_eType = _kreType;
	this->m_iValueMin = _kriMin;
	this->m_iValueMax = _kriMax;
	this->m_iValue = _kriValue;

	// Ensure values are in appropriate ranges
	this->m_iValueMin = std::min( this->m_iValueMin, this->m_iValueMax );
	this->CalculateModifiedMaxStatic();
	this->ClampValue();
}

void UAttribute::Process( float _fDeltaTime )
{
	// When attribute is in a regeneration state
	if ( this->m_bInRegen )
	{
		// Check if enough time has passed to match the regen rate
		this->m_fRegenElapsed += _fDeltaTime;
		if ( this->m_fRegenElapsed > this->m_fRegenRate )
		{
			// Add to the value, enusring that it isn't over the max value
			this->m_iValue = std::min( this->m_iValue + m_iRegenAmount, this->m_iValueModifiedMax );
			this->m_fRegenElapsed -= this->m_fRegenRate;
			this->m_tOnAttributeChanged.Broadcast();
		}

		// Take from the countdown and reset if regen over
		this->m_fRegenTime -= _fDeltaTime;
		if ( this->m_fRegenTime < 0.0f || this->m_iValue == this->m_iValueModifiedMax )
		{
			this->m_bInRegen = false;
			this->m_fRegenTime = 0.0f;
			this->m_fRegenElapsed = 0.0f;

			this->m_tOnRegenEnd.Broadcast();
		}
	}
}

FString UAttribute::GetName() const
{
	return ( this->m_strName );
}

UAttribute::EAttributeType UAttribute::GetType() const
{
	return ( this->m_eType );
}

void UAttribute::SetType( const EAttributeType& _kreType )
{
	this->m_eType = _kreType;
}

int32 UAttribute::GetMin() const
{
	return ( this->m_iValueMin );
}

void UAttribute::SetMin( const int32& _kriMin )
{
	this->m_iValueMin = std::min( _kriMin, this->m_iValueMax );
	this->ClampValue();
}

int32 UAttribute::GetMax() const
{
	return ( this->m_iValueModifiedMax );
}

void UAttribute::SetMax( const int32& _kriMax )
{
	this->m_iValueMax = std::max( _kriMax, this->m_iValueMin );
	switch ( this->m_eType )
	{
		case EAttributeType::ET_STATIC:
		{
			this->CalculateModifiedMaxStatic();
		} break;

		case EAttributeType::ET_DYNAMIC:
		{
			this->CalculateModifiedMaxDynamic();
		} break;

		default: break;
	}
}

int32 UAttribute::GetValue() const
{
	return ( this->m_eType == EAttributeType::ET_STATIC ? this->m_iValueModifiedMax : this->m_iValue );
}

void UAttribute::SetValue( const int32& _kriValue )
{
	// Set appropriate value based on type
	switch ( this->m_eType )
	{
		case EAttributeType::ET_STATIC:
		{
			this->m_iValueMax = std::max( this->m_iValueMax, this->m_iValueMin );
			this->CalculateModifiedMaxStatic();
		} break;

		case EAttributeType::ET_DYNAMIC:
		{
			this->m_iValue = std::min( std::max( _kriValue, this->m_iValueMin ), this->m_iValueModifiedMax );
		} break;
		
		default: break;
	}
}

float UAttribute::GetRegenRate() const
{
	return ( this->m_fRegenRate );
}

void UAttribute::SetRegenRate( const float& _krfRate )
{
	this->m_fRegenRate = _krfRate;
}

FModifiedDelegate& UAttribute::GetOnModifiedDelegate()
{
	return ( this->m_tOnModified );
}

FAlterFailDelegate& UAttribute::GetOnAlterFailDelegate()
{
	return ( this->m_tOnAlterFail );
}

FRegenNotify& UAttribute::GetOnRegenBeginDelegate()
{
	return ( this->m_tOnRegenBegin );
}

FRegenNotify& UAttribute::GetOnRegenEndDelegate()
{
	return ( this->m_tOnRegenEnd );
}

FAttributeChanged& UAttribute::GetOnAttributeChangedDelegate()
{
	return ( this->m_tOnAttributeChanged );
}

void UAttribute::Add( int32 _iAmount )
{
	this->m_iValue += _iAmount;
	this->ClampValue();
}

void UAttribute::Remove( int32 _iAmount )
{
	this->m_iValue -= _iAmount;
	this->ClampValue();
}

void UAttribute::Regen( float _fTime, float _fRate, int32 _iRegenAmount )
{
	if ( !this->m_bInRegen )
	{
		this->m_fRegenElapsed = 0.0f;
	}

	this->m_bInRegen = true;
	this->m_fRegenTime = _fTime;
	this->m_fRegenRate = _fRate;
	this->m_iRegenAmount = _iRegenAmount;

	this->m_tOnRegenBegin.Broadcast();
}

bool UAttribute::AttachModifier( class UAttributeModifier* _pModifier )
{
	bool bReturn{ true };

	// Try locate the modifier in array
	int iIndex{ 0 };
	for ( ; iIndex < this->m_arrModifiers.Num() && this->m_arrModifiers[ iIndex ] != _pModifier; iIndex++ );

	// Ensure that modifier is valid and isn't added twice
	if ( this->m_strName != _pModifier->GetAttributeName() ||
		 iIndex != this->m_arrModifiers.Num() )
	{
		bReturn = false;
	}
	else
	{
		// Add to modifiers list and bind to delegate
		this->m_arrModifiers.Add( _pModifier );

		// Recalculate max value
		switch ( this->m_eType )
		{
			case EAttributeType::ET_STATIC:
			{
				this->CalculateModifiedMaxStatic();
				this->m_tOnAttributeChanged.Broadcast();
			} break;

			case EAttributeType::ET_DYNAMIC:
			{
				this->CalculateModifiedMaxDynamic();
				this->m_tOnAttributeChanged.Broadcast();
			} break;

			default: break;
		}
	}

	return ( bReturn );
}

bool UAttribute::DetachModifier( class UAttributeModifier* _pModifier )
{
	bool bReturn{ false };

	// Attempt to remove modifier
	if ( this->m_arrModifiers.RemoveSingle( _pModifier ) == 1 )
	{
		// Update value based on attribute type
		switch ( this->m_eType )
		{
			case EAttributeType::ET_STATIC:
			{
				this->CalculateModifiedMaxStatic();
				this->m_tOnAttributeChanged.Broadcast();
				bReturn = true;
			} break;

			case EAttributeType::ET_DYNAMIC:
			{
				// Ensure modified doesn't reduce attribute below min
				if ( this->CalculateModifiedMaxDynamic() )
				{
					this->m_tOnAttributeChanged.Broadcast();
					bReturn = true;
				}
				else
				{
					// Add modifier back 
					this->m_arrModifiers.Add( _pModifier );
				}
			} break;

			default: break;
		}
	}

	return ( bReturn );
}

void UAttribute::ClampValue()
{
	this->m_iValue = std::min( std::max( this->m_iValue, this->m_iValueMin ), this->m_iValueModifiedMax );
}

void UAttribute::CalculateModifiedMaxStatic()
{
	// Add up total percentage amount ( with base of 100 )
	float fPercentage{ 100 };
	for ( auto& aModifier : this->m_arrModifiers )
	{
		fPercentage += static_cast< float >( aModifier->GetAmount() );
	}

	// Use percentage as multiplyer to get modified max
	this->m_iValueModifiedMax = static_cast< int32 >( static_cast< float >( this->m_iValueMax ) * ( fPercentage / 100.0f ) );
	this->m_iValue = this->m_iValueModifiedMax;

	this->m_tOnModified.Broadcast( this->m_iValueModifiedMax );
}

bool UAttribute::CalculateModifiedMaxDynamic()
{
	bool bReturn{ false };

	int32 iAmountLost{ this->m_iValueModifiedMax - this->m_iValue };

	// Add up total percentage amount ( with base of 100 )
	float fPercentage{ 100 };
	for ( auto& aModifier : this->m_arrModifiers )
	{
		fPercentage += static_cast< float >( aModifier->GetAmount() );
	}

	// Use percentage as multiplyer to get modified max
	int iValueModifiedMax{ static_cast< int32 >( static_cast< float >( this->m_iValueMax ) * ( fPercentage / 100.0f ) ) };
	if ( iValueModifiedMax - iAmountLost > this->m_iValueMin )
	{
		this->m_iValueModifiedMax = iValueModifiedMax;
		this->m_iValue = this->m_iValueModifiedMax - iAmountLost;

		this->m_tOnModified.Broadcast( this->m_iValueModifiedMax );
		bReturn = true;
	}
	else
	{
		this->m_tOnAlterFail.Broadcast();
	}

	return ( bReturn );
}