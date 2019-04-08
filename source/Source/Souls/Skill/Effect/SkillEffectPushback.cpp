// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

//This Include
#include "SkillEffectPushback.h"

// Implementation
ASkillEffectPushback::ASkillEffectPushback( const FObjectInitializer& _krtInitialiser )
{
	this->m_pPushbackArea = _krtInitialiser.CreateDefaultSubobject< USphereComponent >( this, "PushbackArea" );
	this->RootComponent = this->m_pPushbackArea;
	this->m_pPushbackArea->SetCollisionProfileName( FName( "PushbackArea" ) );
	this->m_pPushbackArea->SetSphereRadius( 1.0f, true );

	this->PrimaryActorTick.bCanEverTick = true;
}

void ASkillEffectPushback::Initialise( float _fDuration, float _fRadius )
{
	this->m_fDuration = _fDuration;
	this->m_fRadius = _fRadius;
}

void ASkillEffectPushback::Tick( float _fDeltaTime )
{
	this->m_fTimeElapsed += _fDeltaTime;
	if ( this->m_fTimeElapsed <= this->m_fDuration )
	{
		float fSize{ ( ( 1.0f / this->m_fDuration ) * this->m_fTimeElapsed ) * this->m_fRadius };
		this->m_pPushbackArea->SetSphereRadius( fSize, true );
	}
	else
	{
		this->Destroy();
	}
}