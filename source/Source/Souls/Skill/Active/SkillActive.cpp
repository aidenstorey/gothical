// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes
#include <algorithm>

// Local Includes
#include "../../Player/Controller/ControllerPlayer.h"

//This Include
#include "SkillActive.h"

// Implementation
float USkillActive::GetCooldown() const
{
	return ( this->m_fCooldown );
}

void USkillActive::SetCooldown( const float& _krfCooldown, const bool& _krbUpdateTimeRemaining )
{
	this->m_fCooldown = _krfCooldown;
	if ( _krbUpdateTimeRemaining )
	{
		this->m_fTimeRemaining = std::min( this->m_fTimeRemaining, this->m_fCooldown );
	}
}

float USkillActive::GetTimeRemaining() const
{
	return ( this->m_fTimeRemaining );
}

void USkillActive::SetTimeRemaining( const float& _krfTimeRemaining, const bool& _krbClampToCooldown )
{
	this->m_fTimeRemaining = _krfTimeRemaining;
	if ( _krbClampToCooldown )
	{
		this->m_fTimeRemaining = std::min( this->m_fTimeRemaining, this->m_fCooldown );
	}
}

float USkillActive::GetCooldownPercentage() const
{
	return ( this->m_fTimeRemaining / this->m_fCooldown );
}

bool USkillActive::OnCooldown() const
{
	return ( this->m_fTimeRemaining > 0.0f );
}

bool USkillActive::Process( const float& _krfDeltaTime )
{
	bool bReturn{ true };

	if ( this->OnCooldown() )
	{
		this->m_fTimeRemaining -= _krfDeltaTime;
		if ( this->m_fTimeRemaining <= 0.0f )
		{
			this->m_fTimeRemaining = 0.0f;
		}
	}

	bReturn &= this->ReceiveProcess( _krfDeltaTime );

	return ( bReturn );
}

bool USkillActive::ReceiveInterupt()
{
	bool bReturn{ false };

	// Check if skill hasn't already been triggered
		// True - stop the skill from being active 
	if ( !this->m_bTriggered )
	{
		this->m_bActivated = false;
		this->m_fTimeRemaining = 0.0f;
		bReturn = true;
	}

	return ( bReturn );
}