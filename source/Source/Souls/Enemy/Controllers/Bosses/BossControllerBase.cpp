// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

// This Includes
#include "BossControllerBase.h"

// Implementation

// Constructor
ABossControllerBase::ABossControllerBase( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
	, m_eCurrentBossState( EBossState::BOSS_STATE_IDLE )
{

	for( int i = 0; i < 3; ++i )
	{
		this->m_fCooldowns[ i ] = 0.0f;
	}
}

void ABossControllerBase::Tick( float DeltaSeconds )
{
	AEnemyControllerBase::Tick( DeltaSeconds );

	for( int i = 0; i < 3; ++i )
	{
		if( this->m_fCooldowns[ i ] > 0.0f )
		{
			this->m_fCooldowns[ i ] -= DeltaSeconds;
		}
	}
}

// Accessor Function
// Returns the current value of m_eCurrentBossState
EBossState ABossControllerBase::GetCurrentBossState() const
{
	return( this->m_eCurrentBossState );
}

// Accessor Function
// Sets the current value of m_eCurrentBossState
void ABossControllerBase::SetCurrentBossState( const EBossState& _krtNewState )
{
	this->m_eCurrentBossState = _krtNewState;
	this->OnStateChange( m_eCurrentBossState );
}

// Accessor Function
// Returns the current value of m_fCooldowns[ _kriAttackIndex ]
float ABossControllerBase::GetBossCooldown( const int& _kreAttackIndex )
{
	return( this->m_fCooldowns[ _kreAttackIndex ] );
}

// Accessor Function
// Sets the current value of m_fCooldowns[ _kriAttackIndex ]
void ABossControllerBase::SetBossCooldown( const int& _kreAttackIndex, const float& _krfNewCooldown )
{
	this->m_fCooldowns[ _kreAttackIndex ] = _krfNewCooldown;
}

// Virtual Function
// Intended to be overridden
void ABossControllerBase::ProcessState()
{

}

// Virtual Function
// Intended to be overridden
FVector ABossControllerBase::GetGoToLocation()
{
	return( FVector( 0.0f, 0.0f, 0.0f ) );
}

void ABossControllerBase::EnterHitState( float _fAnimDuration )
{
	if( m_fHitCooldown <= 0.0f )
	{
		switch( this->GetCurrentBossState() )
		{
		// TODO - Ethan : Data data data
		case EBossState::BOSS_STATE_PRE_ATTACK_A:
		case EBossState::BOSS_STATE_ATTACK_A:
		case EBossState::BOSS_STATE_POST_ATTACK_A:
			this->SetBossCooldown( 0, 3.0f );
			break;

		case EBossState::BOSS_STATE_PRE_ATTACK_B:
		case EBossState::BOSS_STATE_ATTACK_B:
		case EBossState::BOSS_STATE_POST_ATTACK_B:
			this->SetBossCooldown( 1, 3.0f );
			break;

		case EBossState::BOSS_STATE_PRE_ATTACK_C:
		case EBossState::BOSS_STATE_ATTACK_C:
		case EBossState::BOSS_STATE_POST_ATTACK_C:
			return;
			break;

		default: break;
		}

		m_fHitCooldown = _fAnimDuration;
		this->SetCurrentBossState( EBossState::BOSS_STATE_PRE_HIT );
		this->SetCurrentCooldown( 0.0f );
	}
}