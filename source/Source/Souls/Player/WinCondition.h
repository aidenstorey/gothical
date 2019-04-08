// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "WinCondition.generated.h"

// Prototypes
UENUM( BlueprintType )
enum class EWinCondition 
{
	WC_NONE = 0,
	WC_GREAT_KNIGHT = 1,
	WC_CATHEDERAL_GUARDIAN = 2,
	WC_CHIROPTERA = 4,
};

inline EWinCondition operator | ( const EWinCondition& _kreA, const EWinCondition& _kreB )
{
	return ( static_cast< EWinCondition >( static_cast< int >( _kreA ) | static_cast< int >( _kreB ) ) );
}

inline EWinCondition operator & ( const EWinCondition& _kreA, const EWinCondition& _kreB )
{
	return ( static_cast< EWinCondition >( static_cast< int >( _kreA ) & static_cast< int >( _kreB ) ) );
}

inline EWinCondition operator ^ ( const EWinCondition& _kreA, const EWinCondition& _kreB )
{
	return ( static_cast< EWinCondition >( static_cast< int >( _kreA ) ^ static_cast< int >( _kreB ) ) );
}

inline EWinCondition& operator |= ( EWinCondition& _reA, const EWinCondition& _kreB )
{
	return ( _reA = static_cast< EWinCondition >( static_cast< int >( _reA ) | static_cast< int >( _kreB ) ) );
}

inline EWinCondition& operator &= ( EWinCondition& _reA, const EWinCondition& _kreB )
{
	return ( _reA = static_cast< EWinCondition >( static_cast< int >( _reA ) & static_cast< int >( _kreB ) ) );
}

inline EWinCondition& operator ^= ( EWinCondition& _reA, const EWinCondition& _kreB )
{
	return ( _reA = static_cast< EWinCondition >( static_cast< int >( _reA ) ^ static_cast< int >( _kreB ) ) );
}

inline bool WinConditionHasFlag( const EWinCondition& _kreA, const EWinCondition& _kreB )
{
	return ( ( _kreA & _kreB ) != EWinCondition::WC_NONE );
}

inline FString WinConditionToString( const EWinCondition& _kreCondition )
{
	FString tReturn{};

	if ( WinConditionHasFlag( _kreCondition, EWinCondition::WC_GREAT_KNIGHT ) )
	{
		tReturn += "Great Knight ";
	}

	if ( WinConditionHasFlag( _kreCondition, EWinCondition::WC_CATHEDERAL_GUARDIAN ) )
	{
		tReturn += "Cathederal Guardian ";
	}

	if ( WinConditionHasFlag( _kreCondition, EWinCondition::WC_CHIROPTERA ) )
	{
		tReturn += "Chiroptera ";
	}

	return ( tReturn );
}