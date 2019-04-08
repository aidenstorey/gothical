// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/AttributeModifier.h"
#include "../../Attribute/Characteristics.h"

//This Include
#include "SkillPassive.h"

// Implementation
USkillPassive::USkillPassive( const FObjectInitializer& _krtInitialiser )
{
	ConstructorHelpers::FObjectFinder< UDataTable > Data_BP( TEXT( "DataTable'/Game/Data/PassiveSkills'" ) );
	m_DataTable = Data_BP.Object;
}

int32 USkillPassive::PassiveIncreaseCount() const
{
	return ( this->m_arrModifiers.Num() );
}

bool USkillPassive::HasHealthModifier() const
{
	return ( this->HasModifier( "Health" ) );
}

bool USkillPassive::HasDamageModifier() const
{
	return ( this->HasModifier( "Damage" ) );
}

bool USkillPassive::HasAttackSpeedModifier() const
{
	return ( this->HasModifier( "Attack Speed" ) );
}

bool USkillPassive::HasMovementSpeedModifier() const
{
	return ( this->HasModifier( "Movement Speed" ) );
}

bool USkillPassive::HasModifier( FString _tName ) const
{
	int iModifier{ 0 };
	for ( ; iModifier < this->m_arrModifiers.Num() && !( _tName.Compare( this->m_arrModifiers[ iModifier ]->GetAttributeName() ) == 0 ) ; iModifier++ );

	return ( iModifier != this->m_arrModifiers.Num() );
}

void USkillPassive::CustomiseModifiers( int32 _iHealth, int32 _iDamage, int32 _iAttackSpeed, int32 _iMovementSpeed )
{
	this->AddModifier( "Health", _iHealth );
	this->AddModifier( "Damage", _iDamage );
	this->AddModifier( "Attack Speed", _iAttackSpeed );
	this->AddModifier( "Movement Speed", _iMovementSpeed );
	this->m_bCustomCreated = true;
}

void USkillPassive::ReceiveInitialise()
{
	this->m_eType = ESkillType::ST_PASSIVE;
	this->m_tDisplayName = FText::FromString( "Passive" );

	if ( !this->m_bCustomCreated )
	{
		TArray< FString > arrAttributeNames;
		arrAttributeNames.Add( "Health" );
		arrAttributeNames.Add( "Damage" );
		arrAttributeNames.Add( "Attack_Speed" );
		arrAttributeNames.Add( "Movement_Speed" );

		int32 iAttributeCount{ arrAttributeNames.Num() };
		for ( int32 iCount{ 0 }; iCount < iAttributeCount; iCount++ )
		{
			int32 iRandom{ FMath::Rand() % iAttributeCount };

			FString tRandom{ arrAttributeNames[ iRandom ] };
			arrAttributeNames[ iRandom ] = arrAttributeNames[ iCount ];
			arrAttributeNames[ iCount ] = tRandom;
		}

		int32 iModifierChance{ 10 }, iAttributeIndex{ 0 };
		do
		{
			auto tName = arrAttributeNames[ iAttributeIndex ];
			auto pModifierData = m_DataTable->FindRow< FPassiveSkillData >( *tName, TEXT( "LookUpOperation" ) );

			for ( auto& rLetter : tName )
			{
				if ( rLetter == '_' )
				{
					rLetter = ' ';
				}
			}
			
			int32 iRange{ pModifierData->m_iModifierMax - pModifierData->m_iModifierMin };
			int32 iIncrements{ iRange / pModifierData->m_iModifierOffset };
			
			this->AddModifier( tName, ( ( FMath::Rand() % iIncrements + 1 ) * pModifierData->m_iModifierOffset ) + pModifierData->m_iModifierMin );
			iAttributeIndex++;
		} while ( iAttributeIndex < iAttributeCount && FMath::Rand() % iModifierChance == 0 );
	}

	FString strToolTip{};
	for ( auto& pModifier : this->m_arrModifiers )
	{
		strToolTip.Append( "+" + FString::FromInt( pModifier->GetAmount() ) );
		strToolTip.Append( "% " + pModifier->GetAttributeName() + "\n" );
	}

	this->m_tToolTip = FText::FromString( strToolTip );
}

bool USkillPassive::ReceiveActivate()
{
	bool bReturn{ true };

	if ( this->m_pOwningController == nullptr )
	{
		bReturn = false;
	}
	else
	{
		for ( auto& pModifier : this->m_arrModifiers )
		{
			this->m_pOwningController->GetCharacteristics()->AttachModifier( pModifier );
		}
	}
	
	return ( true );
}

bool USkillPassive::ReceiveDeactivate()
{
	bool bReturn{ true };

	if ( this->m_pOwningController == nullptr )
	{
		bReturn = false;
	}
	else
	{
		for ( auto& pModifier : this->m_arrModifiers )
		{
			bReturn &= this->m_pOwningController->GetCharacteristics()->DetachModifier( pModifier );
		}

		if ( bReturn == false )
		{
			for ( auto& pModifier : this->m_arrModifiers )
			{
				this->m_pOwningController->GetCharacteristics()->AttachModifier( pModifier );
			}
		}
	}

	return ( bReturn );
}

bool USkillPassive::ReceiveInterupt()
{
	bool bReturn{ false };

	// ...

	return ( bReturn );
}

void USkillPassive::AddModifier( FString _tName, int32 _iAmount )
{
	if ( _iAmount != 0 )
	{
		auto pModifier = NewObject< UAttributeModifier >( this, *_tName );
		if ( pModifier != nullptr )
		{
			pModifier->Initialise( _tName, _iAmount );
			this->m_arrModifiers.Add( pModifier );
		}
	}
}