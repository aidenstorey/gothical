// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../Characters/BomberCharacter.h"

// This Include
#include "EnemyWispController.h"

// Implementation

// Constructor
AEnemyWispController::AEnemyWispController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	this->SetName( TEXT( "Wisp" ) );
}

// Overridden from AEnemyControllerBase
// Performs state actions and transitions
void AEnemyWispController::ProcessState()
{
	// TODO - Ethan : Magic Numbers
	if( this->GetCurrentCooldown() <= 0.0f )
	{
		switch( this->GetCurrentState() )
		{
		case EEnemyState::ENEMY_STATE_IDLE:
		{
			if( this->DetectPlayer() )
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				this->SetCurrentCooldown( 2.0f );
			}
		}	break;

		case EEnemyState::ENEMY_STATE_SEEKING:
		{
		}	break;

		case EEnemyState::ENEMY_STATE_PRE_EXPLODE:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_EXPLODE );
			this->SetCurrentCooldown( 1.0f );
		}	break;

		case EEnemyState::ENEMY_STATE_EXPLODE:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_EXPLODE );
			Cast< ABomberCharacter >( this->GetPawn() )->Explode();
		}	break;

		// Not really anything to do after you explode
		case EEnemyState::ENEMY_STATE_POST_EXPLODE:
		{

		}	break;

		default: break;
		}
	}

	if( this->GetCurrentState() == EEnemyState::ENEMY_STATE_SEEKING )
	{
		this->MoveToLocation( this->GetGoToLocation() );
	}
}

FVector AEnemyWispController::GetGoToLocation()
{
	// Move towards the player
	return( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() );
}