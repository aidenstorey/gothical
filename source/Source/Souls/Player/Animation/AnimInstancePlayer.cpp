// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

//This Include
#include "AnimInstancePlayer.h"

// Implementation
EAnimStatePlayer UAnimInstancePlayer::GetState()
{
	return ( this->m_eState );
}

EAnimStatePlayer UAnimInstancePlayer::SetState( EAnimStatePlayer _eState )
{
	this->m_eState = _eState;

	this->m_tOnStateChange.Broadcast( this->m_eState );

	return ( this->m_eState );
}

float UAnimInstancePlayer::GetAngle()
{
	return ( this->m_fAngle );
}

void UAnimInstancePlayer::SetAngle( float _fAngle )
{
	this->m_fAngle = _fAngle;
}

FAnimStateChange& UAnimInstancePlayer::GetOnStateChangeDelegate()
{
	return ( this->m_tOnStateChange );
}