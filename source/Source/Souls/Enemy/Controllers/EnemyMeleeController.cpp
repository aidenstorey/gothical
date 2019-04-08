// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../QuarrieDamageBPLibrary.h"
#include "../../Attribute/Characteristics.h"

// This Includes
#include "EnemyMeleeController.h"

// Implementation

// Constructor
AEnemyMeleeController::AEnemyMeleeController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	this->SetName( TEXT( "Melee" ) );

	this->m_SeekDisplacement = FVector( FMath::FRand(), 0.0f, 0.0f ).RotateAngleAxis( FMath::FRandRange( 0.0f, 360.0f ), FVector( 0.0f, 0.0f, 1.0f ) ) * 500.0f;
}

// Overridden from AEnemyControllerBase
// Performs state actions and transitions
void AEnemyMeleeController::ProcessState()
{
	if( this->GetCurrentCooldown() <= 0.0f )
	{
		switch( this->GetCurrentState() )
		{
			case EEnemyState::ENEMY_STATE_IDLE:
			{
				if( this->DetectPlayer() )
				{
					this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				}
			} break;

			case EEnemyState::ENEMY_STATE_SEEKING:
			{
				if( this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) <= 120.0f )
				{
					this->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_ATTACK );
					this->SetCurrentCooldown( 0.5f );
				}
			} break;

			case EEnemyState::ENEMY_STATE_FLEEING:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
			} break;

			case EEnemyState::ENEMY_STATE_PRE_ATTACK:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_ATTACK );
			} break;

			case EEnemyState::ENEMY_STATE_ATTACK:
			{
				this->Attack();
				this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_ATTACK );
				this->SetCurrentCooldown( 0.8f );
			} break;

			case EEnemyState::ENEMY_STATE_POST_ATTACK:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			} break;

			case EEnemyState::ENEMY_STATE_PRE_HIT:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_HIT );
				this->SetCurrentCooldown( this->GetCurrentHitCooldown() );
				this->GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
			} break;

			case EEnemyState::ENEMY_STATE_HIT:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_HIT );
			} break;

			case EEnemyState::ENEMY_STATE_POST_HIT:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				this->SetCurrentHitCooldown( 3.0f );
			} break;

			default: break;
		}
	}

	if( this->GetCurrentState() == EEnemyState::ENEMY_STATE_SEEKING ||
		this->GetCurrentState() == EEnemyState::ENEMY_STATE_FLEEING )
	{
		this->MoveToLocation( this->GetGoToLocation() );
	}

	if( this->GetCurrentState() == EEnemyState::ENEMY_STATE_PRE_ATTACK )
	{
		FRotator RotTemp = ( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetCharacter()->GetActorLocation() ).Rotation();
		RotTemp.Pitch = 0.0f;
		this->GetCharacter()->SetActorRotation( RotTemp );
	}
}

// Overridden from AEnemyControllerBase
// Returns locations to move to
FVector AEnemyMeleeController::GetGoToLocation()
{
	FVector Temp;
	switch( this->GetCurrentState() )
	{
		// TODO - Ethan : Format this switch in Aiden's stupid way
		// Return location of the player
	case EEnemyState::ENEMY_STATE_SEEKING:
		Temp = UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation();
		if( this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) > 600.0f )
		{
			Temp += m_SeekDisplacement;
		}
		return( Temp );
		break;

		// Move away from the player
	case EEnemyState::ENEMY_STATE_FLEEING:
		// Get direction from player to self
		Temp = this->GetPawn()->GetActorLocation() - UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation();
		Temp.Normalize();

		// Return position 1000.0f away from player
		return( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() + ( Temp * 1000.0f ) );

		break;

	default:
		return( this->GetPawn()->GetActorLocation() );
		break;
	}
}

// Overridden from AEnemyControllerBase
// Performs attack action
void AEnemyMeleeController::Attack()
{
	UQuarrieDamageBPLibrary::QApplyConicDamage( this->GetWorld(), this->GetCharacteristics()->GetDamage(), this->GetPawn()->GetActorLocation(), 150.0f, 30.0f, this->GetPawn()->GetActorForwardVector(), UDamageType::StaticClass(), TArray<AActor*>(), true, this->GetPawn() );
}