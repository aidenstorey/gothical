// Fill out your copyright notice in the Description page of Project Settings.

#include "Souls.h"
#include "EnemyGhostController.h"

AEnemyGhostController::AEnemyGhostController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	this->SetName( "Ghost" );
}

void AEnemyGhostController::ProcessState()
{
	if( this->GetCurrentCooldown() <= 0.0f )
	{
		switch( this->GetCurrentState() )
		{
		case EEnemyState::ENEMY_STATE_IDLE:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_WANDER );
		}	break;
		case EEnemyState::ENEMY_STATE_WANDER:
		{
			if( this->DetectPlayer() )
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				this->SetCurrentCooldown( 2.7f );
			}
		}	break;
		case EEnemyState::ENEMY_STATE_SEEKING:
		{
			if( this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) <= 600.0f )
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_ATTACK );
				this->SetCurrentCooldown( 0.0f );
			}
		}	break;
		case EEnemyState::ENEMY_STATE_PRE_ATTACK:
		{
			FVector vecTemp = UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetPawn()->GetActorLocation();
			vecTemp.Normalize();

			this->m_ChargeDestination = UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() + ( vecTemp * 500.0f );

			this->SetCurrentState( EEnemyState::ENEMY_STATE_ATTACK );
			this->SetCurrentCooldown( 2.467f );
		}	break;
		case EEnemyState::ENEMY_STATE_ATTACK:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_ATTACK );
		}	break;
		case EEnemyState::ENEMY_STATE_POST_ATTACK:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
			this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
		}	break;
		default:
		{}	break;
		}
	}

	switch( this->GetCurrentState() )
	{
	case EEnemyState::ENEMY_STATE_WANDER:
	{
		this->MoveTo( GetGoToLocation() );
	}	break;

	case EEnemyState::ENEMY_STATE_ATTACK:
	{
		FVector tLookAt{ this->m_ChargeDestination - this->GetPawn()->GetActorLocation() };
		FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };

		this->GetCharacter()->SetActorRotation( tLookAtRotation );

		this->GetCharacter()->AddMovementInput( this->GetCharacter()->GetActorForwardVector() );
	}	break;

	// Intentional fall through
	case EEnemyState::ENEMY_STATE_SEEKING:
	{
		this->MoveTo( GetGoToLocation() );
	}
	default:
	{
		FVector tLookAt{ UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetPawn()->GetActorLocation() };
		FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };

		this->GetCharacter()->SetActorRotation( tLookAtRotation );
	}	break;

	}
}

FVector AEnemyGhostController::GetGoToLocation()
{
	FVector vecTemp;

	if( this->GetCurrentState() == EEnemyState::ENEMY_STATE_WANDER )
	{
		vecTemp = this->GetPawn()->GetActorForwardVector() * 50.0f;
		vecTemp = vecTemp.RotateAngleAxis( 1.0f, this->GetPawn()->GetActorUpVector() );

		vecTemp += this->GetPawn()->GetActorLocation();

		return( vecTemp );
	}
	else
	{
		// Get direction from player to self
		vecTemp = this->GetPawn()->GetActorLocation() - UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation();
		vecTemp.Normalize();

		if( this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) < 750.0f )
		{
			vecTemp = vecTemp.RotateAngleAxis( 10.0f, UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorUpVector() );
		}

		// Return position 500.0f away from player, rotated around player
		return( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() + ( vecTemp * 500.0f ) );
	}
}