// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "Player/Controller/ControllerPlayer.h"

//This Include
#include "SoulsGameMode.h"

// Implementation
ASoulsGameMode::ASoulsGameMode( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	this->PlayerControllerClass = AControllerPlayer::StaticClass();

	static ConstructorHelpers::FClassFinder< APawn > PlayerPawnBPClass( TEXT( "/Game/Blueprints/Player/BP_Player" ) );
	if ( PlayerPawnBPClass.Class != NULL )
	{
		this->DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

bool ASoulsGameMode::GetWinConditionDisabled() const
{
	return ( this->m_bWinConditionDisabled );
}

void ASoulsGameMode::SetWinConditionDisabled( bool _bDisabled )
{
	this->m_bWinConditionDisabled = _bDisabled;
	if ( !this->m_bWinConditionDisabled )
	{
		this->CheckGameWon();
	}
}

EWinCondition ASoulsGameMode::GetWinCondition() const
{
	return ( this->m_eConditionWin );
}

void ASoulsGameMode::SetWinCondition( EWinCondition _eCondition )
{
	this->m_eConditionWin = _eCondition;
}

EWinCondition ASoulsGameMode::GetRemainingConditions() const
{
	return ( this->m_eConditionRemaining );
}

void ASoulsGameMode::SetRemainingConditions( EWinCondition _eCondition, bool _bCheck )
{
	this->m_eConditionRemaining = _eCondition;

	if ( _bCheck )
	{
		this->CheckGameWon();
	}
}

FGameWonDelegate& ASoulsGameMode::GetOnGameWonDelegate()
{
	return ( this->m_tOnGameWon );
}

void ASoulsGameMode::StartGame( EWinCondition _eCondition )
{
	this->m_eConditionWin = _eCondition;
	this->m_eConditionRemaining = this->m_eConditionWin;
}

void ASoulsGameMode::CheckGameWon()
{
	if ( this->m_eConditionRemaining == EWinCondition::WC_NONE && !this->m_bWinConditionDisabled )
	{
		this->m_tOnGameWon.Broadcast();
	}
}

void ASoulsGameMode::WinConditionMet( EWinCondition _eCondition )
{
	this->m_eConditionRemaining ^= _eCondition;
	this->CheckGameWon();
}

bool ASoulsGameMode::CheckWinConditionMet( EWinCondition _eCondition )
{
	return ( !WinConditionHasFlag( this->m_eConditionRemaining, _eCondition ) );
}

void ASoulsGameMode::AddCondition( EWinCondition _eCondition )
{
	this->m_eConditionWin |= _eCondition;
	this->m_eConditionRemaining |= _eCondition;
}

void ASoulsGameMode::RemoveCondition( EWinCondition _eCondition )
{
	this->m_eConditionWin ^= _eCondition;
	this->m_eConditionRemaining ^= _eCondition;

	this->CheckGameWon();
}