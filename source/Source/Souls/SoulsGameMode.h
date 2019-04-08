// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/GameMode.h"

// Library Includes

// Local Includes
#include "Player/WinCondition.h"

//Generated Include
#include "SoulsGameMode.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FGameWonDelegate );

// Prototypes
UCLASS()
class ASoulsGameMode : public AGameMode
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructors
	ASoulsGameMode( const FObjectInitializer& _krtInitialiser );

	// Accessors
	bool GetWinConditionDisabled() const;
	void SetWinConditionDisabled( bool _bDisabled );

	EWinCondition GetWinCondition() const;
	void SetWinCondition( EWinCondition _eCondition );

	EWinCondition GetRemainingConditions() const;
	void SetRemainingConditions( EWinCondition _eCondition, bool _bCheck = true );

	FGameWonDelegate& GetOnGameWonDelegate();

	// Other
	void StartGame( EWinCondition _eCondition );
	void CheckGameWon();
	void WinConditionMet( EWinCondition _eCondition );
	bool CheckWinConditionMet( EWinCondition _eCondition );

	void AddCondition( EWinCondition _eCondition );
	void RemoveCondition( EWinCondition _eCondition );

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() bool m_bWinConditionDisabled;
	EWinCondition m_eConditionWin;
	EWinCondition m_eConditionRemaining;

	FGameWonDelegate m_tOnGameWon;
};
