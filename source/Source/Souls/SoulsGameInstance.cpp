// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "Skill/SkillManager.h"

//This Include
#include "SoulsGameInstance.h"

// Implementation
USoulsGameInstance::USoulsGameInstance()
{
	this->m_pSkillManager = NewObject< USkillManager >( this, "SkillManager" );
}

bool USoulsGameInstance::GetIsStartUp() const
{
	return ( this->m_bIsStartUp );
}

void USoulsGameInstance::SetIsStartUp( const bool& _krbStartUp )
{
	this->m_bIsStartUp = _krbStartUp;
}

bool USoulsGameInstance::GetShowDebugScreen() const
{
	return ( this->m_bShowDebugScreen );
}

void USoulsGameInstance::SetShowDebugScreen( const bool& _krbShow )
{
	this->m_bShowDebugScreen = _krbShow;
}

USkillManager* USoulsGameInstance::GetSkillManager()
{
	return ( this->m_pSkillManager );
}

void USoulsGameInstance::Init()
{
	this->m_bIsStartUp = true;
}

void USoulsGameInstance::ToggleDebugScreen()
{
	this->m_bShowDebugScreen = !this->m_bShowDebugScreen;
}