// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Base/ProjectileBase.h"

// This Include
#include "EnemyRangedController.h"

// Implementation

// Constructor
AEnemyRangedController::AEnemyRangedController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	this->SetName( TEXT( "Ranged" ) );
}

// Overridden from AEnemyControllerBase
// Performs state actions and transitions
void AEnemyRangedController::ProcessState()
{
	FVector tLookAt{ UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetPawn()->GetActorLocation() };
	FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };

	this->GetPawn()->SetActorRotation( tLookAtRotation );


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
					this->SetCurrentCooldown( 3.0f );
				}
			}	break;

			case EEnemyState::ENEMY_STATE_SEEKING:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_ATTACK );
				this->SetCurrentCooldown( 2.5f );
			}	break;

			case EEnemyState::ENEMY_STATE_PRE_ATTACK:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_ATTACK );
				this->SetCurrentCooldown( 1.0f );
			}	break;

			case EEnemyState::ENEMY_STATE_ATTACK:
			{
				this->Attack();
				this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_ATTACK );
				this->SetCurrentCooldown( 1.0f );
			}	break;

			case EEnemyState::ENEMY_STATE_POST_ATTACK:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			default: break;
		}
	}

	if( this->GetCurrentState() == EEnemyState::ENEMY_STATE_SEEKING )
	{
		this->MoveToLocation( this->GetGoToLocation() );
	}
}

// Overridden from AEnemyControllerBase
// Returns locations to move to
FVector AEnemyRangedController::GetGoToLocation()
{
	FVector vecTemp;

	// Get direction from player to self
	vecTemp = this->GetPawn()->GetActorLocation() - UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation();
	vecTemp.Normalize();

	// Return position 500.0f away from player
	return( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() + ( vecTemp * 500.0f ) );
}

// Overridden from AEnemyControllerBase
// Performs attack action
void AEnemyRangedController::Attack()
{
	if( this->m_ProjectileReference != NULL )
	{
		AProjectileBase* pTempProjectile = Cast< AProjectileBase >( UGameplayStatics::BeginSpawningActorFromClass( this, this->m_ProjectileReference, FTransform( this->GetPawn()->GetActorRotation(), this->GetPawn()->GetActorLocation() + ( this->GetActorForwardVector() * 100.0f ) ) ) );
	
		if( pTempProjectile )
		{
	
			UGameplayStatics::FinishSpawningActor( pTempProjectile, FTransform( this->GetPawn()->GetActorRotation(), this->GetPawn()->GetActorLocation() + ( this->GetActorForwardVector() * 100.0f ) ) );
			pTempProjectile->Initialize( this, 1500.0f, this->GetCharacteristics()->GetDamage(), 2.0f );
		}
	}
}