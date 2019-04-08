// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "Active/SkillEarthquake.h"
#include "Active/SkillFireball.h"
#include "Active/SkillPoison.h"
#include "Active/SkillHealer.h"
#include "Passive/SkillPassive.h"

//This Include
#include "SkillManager.h"

// Implementation
USkillManager::USkillManager()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillManager'" ) };
	if ( pSkillData.Succeeded() )
	{
		FString tName{ "Initial" };
		auto pData = pSkillData.Object->FindRow< FSkillManagerData >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_mapSkillClasses.Empty( pData->m_arrSkillClasses.Num() );

			for ( auto& pSkill : pData->m_arrSkillClasses )
			{
				this->m_mapSkillClasses.Add( pSkill.m_eType, pSkill.m_pSkill );
			}

			this->m_iPassiveMaxCount = pData->m_iPassiveMaxCount;
		}
	}

	
}

void USkillManager::Initialise()
{
	this->m_arrSkillClasses.Empty();
	
	for ( int iPassive{ 0 }; iPassive < this->m_iPassiveMaxCount; iPassive++ )
	{
		this->m_arrSkillClasses.Add( this->m_mapSkillClasses[ ESkillClassType::ST_PASSIVE ] );
	}

	for ( auto& aSkillClass : this->m_mapSkillClasses )
	{
		if ( aSkillClass.Value != USkillPassive::StaticClass() )
		{
			this->m_arrSkillClasses.Add( aSkillClass.Value );
		}
	}

	// Randomise array
	int32 iSkillCount{ this->m_arrSkillClasses.Num() };
	for ( int32 iCount{ 0 }; iCount < iSkillCount; iCount++ )
	{
		int32 iRandom{ FMath::Rand() % iSkillCount };

		UClass* pRandom{ this->m_arrSkillClasses[ iRandom ] };
		this->m_arrSkillClasses[ iRandom ] = this->m_arrSkillClasses[ iCount ];
		this->m_arrSkillClasses[ iCount ] = pRandom;		
	}

	this->m_iNextSkillIndex = 0;
}

USkillBase* USkillManager::RequestSkill()
{
	USkillBase* pReturn{ nullptr };

	if ( this->m_iNextSkillIndex < this->m_arrSkillClasses.Num() )
	{
		UClass* pSkillClass{ this->m_arrSkillClasses[ this->m_iNextSkillIndex ] };

		if ( pSkillClass != nullptr )
		{
			pReturn = NewObject< USkillBase >( this, pSkillClass );
			pReturn->Initialise();
		}

		this->m_iNextSkillIndex++;
	}

	return ( pReturn );
}

USkillBase* USkillManager::CreateSkill( ESkillClassType _eType )
{
	USkillBase* pReturn{ nullptr };

	if ( this->m_mapSkillClasses.Contains( _eType ) )
	{
		UClass* pSkillClass{ this->m_mapSkillClasses[ _eType ] };
		if ( pSkillClass != nullptr )
		{
			pReturn = NewObject< USkillBase >( this, pSkillClass );
			pReturn->Initialise();
		}
	}

	return ( pReturn );
}

USkillBase* USkillManager::CreateRandomSkill()
{
	USkillBase* pReturn{ nullptr };

	if( this->m_mapSkillClasses.Num() > 0 )
	{
		TArray< ESkillClassType > arrKeys;
		this->m_mapSkillClasses.GetKeys( arrKeys );

		UClass* pSkillClass{ this->m_mapSkillClasses[ arrKeys[ FMath::Rand() % arrKeys.Num() ] ] };
		if( pSkillClass != nullptr )
		{
			pReturn = NewObject< USkillBase >( this, pSkillClass );
			pReturn->Initialise();
		}
	}

	return ( pReturn );
}

USkillBase* USkillManager::CreateCustomPassive( int32 _iHealth, int32 _iDamage, int32 _iAttackSpeed, int32 _iMovementSpeed )
{
	auto pPassive = NewObject< USkillPassive >( this, USkillPassive::StaticClass() );
	if ( pPassive )
	{
		pPassive->CustomiseModifiers( _iHealth, _iDamage, _iAttackSpeed, _iMovementSpeed );
		pPassive->Initialise();
	}

	return ( pPassive );
}

bool USkillManager::IsSkillAvailable( ESkillClassType _eType )
{
	return ( this->m_mapSkillClasses.Contains( _eType ) );
}