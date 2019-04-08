// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Incldues

// This Include
#include "EnemyRangedWraithController.h"

// Implementation

// Constructor
AEnemyRangedWraithController::AEnemyRangedWraithController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	this->SetName( TEXT( "Wraith" ) );
}

// Overridden from AEnemyRangedController
// Performs state actions and transitions
void AEnemyRangedWraithController::ProcessState()
{
	EEnemyState CurrentState = this->GetCurrentState();

	if( CurrentState != EEnemyState::ENEMY_STATE_WANDER )
	{
		FVector tLookAt{ UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetPawn()->GetActorLocation() };
		FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };

		this->GetPawn()->SetActorRotation( tLookAtRotation );
	}

	if( CurrentState != EEnemyState::ENEMY_STATE_IDLE )
	{
		this->MoveToLocation( this->GetGoToLocation() );
	}
	// TODO - Ethan : Magic Numbers
	if( this->GetCurrentCooldown() <= 0.0f )
	{
		switch( CurrentState )
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
				this->SetCurrentCooldown( 3.0f );
			}
		} break;

		case EEnemyState::ENEMY_STATE_SEEKING:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_ATTACK );
			this->SetCurrentCooldown( 0.0f );
		}	break;

		case EEnemyState::ENEMY_STATE_PRE_ATTACK:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_ATTACK );
			this->SetCurrentCooldown( 1.2f );
		}	break;

		case EEnemyState::ENEMY_STATE_ATTACK:
		{
			this->Attack();
			this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_ATTACK );
			this->SetCurrentCooldown( 0.467f );
		}	break;

		case EEnemyState::ENEMY_STATE_POST_ATTACK:
		{
			this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
			this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
		}	break;

		default: break;
		}
	}

	
}

// Overridden from AEnemyRangedController
// Returns locations to move to
FVector AEnemyRangedWraithController::GetGoToLocation()
{
	FVector vecTemp;

	if( this->GetCurrentState() == EEnemyState::ENEMY_STATE_WANDER )
	{
		vecTemp = this->GetPawn()->GetActorForwardVector() * 50.0f;
		vecTemp = vecTemp.RotateAngleAxis( -1.0f, this->GetPawn()->GetActorUpVector() );

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

// Overridden from AEnemyRangedController
// Performs attack action
void AEnemyRangedWraithController::Attack()
{
	// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, this->GetCharacter()->GetMesh()->GetSocketLocation( "wraith_lantern_socket" ).ToString() );

	if( this->m_ProjectileReference != NULL )
	{
		AProjectileBase* pTempProjectile = Cast< AProjectileBase >( UGameplayStatics::BeginSpawningActorFromClass( this, this->m_ProjectileReference, FTransform( this->GetCharacter()->GetActorRotation(), this->GetPawn()->GetActorLocation() /* this->GetCharacter()->GetMesh()->GetSocketLocation( "wraith_lantern_socket" ) */ + ( this->GetActorRightVector() * 100.0f ) ) ) );

		if( pTempProjectile )
		{

			UGameplayStatics::FinishSpawningActor( pTempProjectile, FTransform( this->GetCharacter()->GetActorRotation(), this->GetPawn()->GetActorLocation() /* this->GetCharacter()->GetMesh()->GetSocketLocation( "wraith_lantern_socket" ) */ + ( this->GetActorRightVector() * 100.0f ) ) );
			pTempProjectile->Initialize( this, 1500.0f, this->GetCharacteristics()->GetDamage(), 2.0f );
		}
	}
}