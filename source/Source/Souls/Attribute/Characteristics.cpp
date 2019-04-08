// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes
#include <algorithm>

// Local Includes
#include "Attribute.h"
#include "AttributeModifier.h"

//This Include
#include "Characteristics.h"

// Implementation
UCharacteristics::UCharacteristics( const FObjectInitializer& _krtInitialiser ) :
	m_bInvincible( false )
{
	ConstructorHelpers::FObjectFinder< UDataTable > Data_BP( TEXT( "DataTable'/Game/Data/Characteristics'" ) );
	m_DataTable = Data_BP.Object;

	// Initialise player variables // TODO:	Make this data driven
	this->m_pHealth = _krtInitialiser.CreateDefaultSubobject< UAttribute >( this, TEXT( "Health" ) );
	this->m_pDamage = _krtInitialiser.CreateDefaultSubobject< UAttribute >( this, TEXT( "Damage" ) );
	this->m_pAttackSpeed = _krtInitialiser.CreateDefaultSubobject< UAttribute >( this, TEXT( "AttackSpeed" ) );
	this->m_pMovementSpeed = _krtInitialiser.CreateDefaultSubobject< UAttribute >( this, TEXT( "MovementSpeed" ) );
	this->m_pRange = _krtInitialiser.CreateDefaultSubobject< UAttribute >( this, TEXT( "Range" ) );
}

int32 UCharacteristics::GetHealth() const
{
	return ( this->m_pHealth->GetValue() );
}

UAttribute* UCharacteristics::GetHealthAttribute()
{
	return ( this->m_pHealth );
}

int32 UCharacteristics::GetDamage() const
{
	return ( this->m_pDamage->GetValue() );
}

UAttribute* UCharacteristics::GetDamageAttribute()
{
	return ( this->m_pDamage );
}

int32 UCharacteristics::GetAttackSpeed() const
{
	return ( this->m_pAttackSpeed->GetValue() );
}

UAttribute* UCharacteristics::GetAttackSpeedAttribute()
{
	return ( this->m_pAttackSpeed );
}

int32 UCharacteristics::GetMovementSpeed() const
{
	return ( this->m_pMovementSpeed->GetValue() );
}

UAttribute* UCharacteristics::GetMovementSpeedAttribute()
{
	return ( this->m_pMovementSpeed );
}

int32 UCharacteristics::GetRange() const
{
	return ( this->m_pRange->GetValue() );
}

UAttribute* UCharacteristics::GetRangeAttribute()
{
	return ( this->m_pRange );
}

float UCharacteristics::GetHealthPercent() const
{
	float fMin{ static_cast< float >( this->m_pHealth->GetMin() ) };
	float fMax{ static_cast< float >( this->m_pHealth->GetMax() ) };
	float fValue{ static_cast< float >( this->m_pHealth->GetValue() ) };

	return ( ( fValue - fMin ) / ( fMax - fMin ) );
}

bool UCharacteristics::GetInvincible() const
{
	return ( this->m_bInvincible );
}

void UCharacteristics::SetInvincible( const bool& _krbInvincible )
{
	this->m_bInvincible = _krbInvincible;
}

FTakeDamage& UCharacteristics::GetOnTakeDamageDelegate()
{
	return ( this->m_tOnTakeDamage );
}

FCharacteristicsNotifyDelegate& UCharacteristics::GetOnDeathDelegate()
{
	return ( this->m_tOnDeath );
}

FCharacteristicsNotifyDelegate& UCharacteristics::GetOnHealDelegate()
{
	return ( this->m_tOnHeal );
}

FHealthChange& UCharacteristics::GetOnHealthChangeDelegate()
{
	return ( this->m_tOnHealthChange );
}

FHealthChange& UCharacteristics::GetOnHealthSetDelegate()
{
	return ( this->m_tOnHealthSet );
}

FPoisonNotify& UCharacteristics::GetOnPoisonBeginDelegate()
{
	return ( this->m_tOnPoisonBegin );
}

FPoisonNotify& UCharacteristics::GetOnPoisonEndDelegate()
{
	return ( this->m_tOnPoisonEnd );
}

void UCharacteristics::Initialise( const FName& _krtRowName )
{
	FCharacteristicData* pTempData = m_DataTable->FindRow< FCharacteristicData >( _krtRowName, TEXT( "LookUpOperation" ) );

	if( pTempData != NULL )
	{
		this->m_pHealth->Initialise( "Health", UAttribute::EAttributeType::ET_DYNAMIC, 0, pTempData->m_iHealth, pTempData->m_iHealth );
		this->m_pDamage->Initialise( "Damage", UAttribute::EAttributeType::ET_STATIC, 0, pTempData->m_iDamage, pTempData->m_iDamage );
		this->m_pAttackSpeed->Initialise( "Attack Speed", UAttribute::EAttributeType::ET_STATIC, 0, pTempData->m_iAttackSpeed, pTempData->m_iAttackSpeed );
		this->m_pMovementSpeed->Initialise( "Movement Speed", UAttribute::EAttributeType::ET_STATIC, 0, pTempData->m_iMovementSpeed, pTempData->m_iMovementSpeed );
		this->m_pRange->Initialise( "Range", UAttribute::EAttributeType::ET_STATIC, 0, pTempData->m_iRange, pTempData->m_iRange );

		// Add attributes to map
		this->m_mapAttributes.Add( this->m_pHealth->GetName(), this->m_pHealth );
		this->m_mapAttributes.Add( this->m_pDamage->GetName(), this->m_pDamage );
		this->m_mapAttributes.Add( this->m_pAttackSpeed->GetName(), this->m_pAttackSpeed );
		this->m_mapAttributes.Add( this->m_pMovementSpeed->GetName(), this->m_pMovementSpeed );
		this->m_mapAttributes.Add( this->m_pRange->GetName(), this->m_pRange );

		this->m_pHealth->GetOnAttributeChangedDelegate().AddDynamic( this, &UCharacteristics::HealthChanged );
	}
}

void UCharacteristics::Process( float _fDeltaTime )
{
	this->m_pHealth->Process( _fDeltaTime );

	// When attribute is in a Poison state
	if ( this->m_bInPoison )
	{
		// Check if enough time has passed to match the Poison rate
		this->m_fPoisonElapsed += _fDeltaTime;
		if ( this->m_fPoisonElapsed > this->m_fPoisonRate )
		{
			// Add to the value, enusring that it isn't over the max value
			this->TakeDamage( this->m_iPoisonAmount );
			this->m_fPoisonElapsed -= this->m_fPoisonRate;
		}

		// Take from the countdown and reset if Poison over
		this->m_fPoisonTime -= _fDeltaTime;
		if ( this->m_fPoisonTime < 0.0f )
		{
			this->m_bInPoison = false;
			this->m_fPoisonTime = 0.0f;
			this->m_fPoisonElapsed = 0.0f;

			this->m_tOnPoisonEnd.Broadcast();
		}
	}
}

void UCharacteristics::Heal()
{
	this->m_pHealth->SetValue( this->m_pHealth->GetMin() );
	this->Heal( this->m_pHealth->GetMax() );
}

void UCharacteristics::Heal( const int32& _kriAmount )
{
	this->m_pHealth->Add( _kriAmount );
	this->m_tOnHeal.Broadcast();
	this->HealthChanged();
}

void UCharacteristics::Heal( const float& _krfPercent )
{
	this->Heal( static_cast< int32 >( static_cast< float >( this->m_pHealth->GetMax() ) / 100.0f * _krfPercent ) );
}

void UCharacteristics::Regen( float _fTime, float _fRate, int32 _iRegenAmount )
{
	this->m_pHealth->Regen( _fTime, _fRate, _iRegenAmount );
}

void UCharacteristics::Poison( float _fTime, float _fRate, int32 _iDamageAmount )
{
	if ( !this->m_bInPoison )
	{
		this->m_fPoisonElapsed = 0.0f;
		this->m_tOnPoisonBegin.Broadcast();
	}

	this->m_bInPoison = true;
	this->m_fPoisonTime = _fTime;
	this->m_fPoisonRate = _fRate;
	this->m_iPoisonAmount = _iDamageAmount;
}

void UCharacteristics::ActivateInvincibility()
{
	this->m_bInvincible = true;
}

void UCharacteristics::DeactivateInvincibility()
{
	this->m_bInvincible = false;
}

void UCharacteristics::Kill()
{
	this->m_pHealth->SetValue( this->m_pHealth->GetMin() );
	this->m_tOnDeath.Broadcast();
}

void UCharacteristics::TakeDamage( const int32& _kriAmount, AActor* _pLastHit )
{
	if ( !this->m_bInvincible )
	{
		this->m_pHealth->Remove( _kriAmount );
		this->m_tOnTakeDamage.Broadcast( _kriAmount );

		this->HealthChanged();

		if ( this->m_pHealth->GetValue() <= this->m_pHealth->GetMin() )
		{
			this->Kill();
		}
	}
}

bool UCharacteristics::AttachModifier( class UAttributeModifier* _pModifier )
{
	bool bReturn{ true };

	if ( !this->m_mapAttributes.Contains( _pModifier->GetAttributeName() ) )
	{
		bReturn = false;
	}
	else
	{
		bReturn &= this->m_mapAttributes[ _pModifier->GetAttributeName() ]->AttachModifier( _pModifier );
	}

	return ( bReturn );
}

bool UCharacteristics::DetachModifier( class UAttributeModifier* _pModifier )
{
	bool bReturn{ true };

	if ( !this->m_mapAttributes.Contains( _pModifier->GetAttributeName() ) )
	{
		bReturn = false;
	}
	else
	{
		bReturn &= this->m_mapAttributes[ _pModifier->GetAttributeName() ]->DetachModifier( _pModifier );
	}

	return ( bReturn );

}

void UCharacteristics::HealthChanged()
{
	this->m_tOnHealthChange.Broadcast( static_cast< float >( this->m_pHealth->GetValue() ) /
									   static_cast< float >( this->m_pHealth->GetMax() ) );
}