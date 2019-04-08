// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

//This Include
#include "CharacterBase.h"

// Implementation
ACharacterBase::ACharacterBase( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	// Set default size for capsule
	this->GetCapsuleComponent()->InitCapsuleSize( 42.0f, 96.0f );
}