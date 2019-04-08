// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

//This Include
#include "AttributeModifier.h"

// Implementation
UAttributeModifier::UAttributeModifier()
{
	// ...
}

FString UAttributeModifier::GetAttributeName() const
{
	return ( this->m_strAttributeName );
}

int32 UAttributeModifier::GetAmount() const
{
	return ( this->m_iAmount );
}

void UAttributeModifier::SetAmount( const int32& _kriAmount )
{
	this->m_iAmount = _kriAmount;
	this->m_tOnAltered.Broadcast();
}

FModifierAltered& UAttributeModifier::GetOnAlteredDelegate()
{
	return ( this->m_tOnAltered );
}

void UAttributeModifier::Initialise( const FAttributeModifierDefinition& _tDefinition )
{
	this->m_strAttributeName = _tDefinition.m_tName;
	this->m_iAmount = _tDefinition.m_iAmount;
}

void UAttributeModifier::Initialise( const FString& _krstrAttributeName, const int32& _kriAmount )
{
	this->m_strAttributeName = _krstrAttributeName;
	this->m_iAmount = _kriAmount;
}