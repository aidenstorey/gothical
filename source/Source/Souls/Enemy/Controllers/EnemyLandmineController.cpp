// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Base/ProjectileBase.h"
#include "../Characters/BomberCharacter.h"

// This Include
#include "EnemyLandmineController.h"

// Implementation

// Constructor
AEnemyLandmineController::AEnemyLandmineController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	this->SetName( TEXT( "Landmine" ) );
}

// Overridden from AEnemyControllerBase
// Performs state actions and transitions
void AEnemyLandmineController::ProcessState()
{
	FVector tLookAt{ UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetPawn()->GetActorLocation() };
	FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };

	if( this->GetCurrentState() != EEnemyState::ENEMY_STATE_IDLE )
	{
		this->GetPawn()->SetActorRotation( tLookAtRotation );
	}

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
				this->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_ATTACK );
				this->SetCurrentCooldown( 1.25f );
			}	break;

			case EEnemyState::ENEMY_STATE_PRE_ATTACK:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_ATTACK );
			}	break;

			case EEnemyState::ENEMY_STATE_ATTACK:
			{
				this->Attack();
				this->SetCurrentState( EEnemyState::ENEMY_STATE_POST_ATTACK );
				this->SetCurrentCooldown( 1.25f );
			}	break;

			case EEnemyState::ENEMY_STATE_POST_ATTACK:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			case EEnemyState::ENEMY_STATE_PRE_EXPLODE:
			{
				this->SetCurrentState( EEnemyState::ENEMY_STATE_EXPLODE );
				this->SetCurrentCooldown( 1.0f );
			}	break;

			case EEnemyState::ENEMY_STATE_EXPLODE:
			{
				Cast< ABomberCharacter >( this->GetPawn() )->Explode();
			}	break;

			// Not really anything to do after you explode
			case EEnemyState::ENEMY_STATE_POST_EXPLODE:
			{

			}	break;

			default: break;
		}
	}
}

// Overridden from AEnemyControllerBase
// Performs attack action
void AEnemyLandmineController::Attack()
{
	if( this->m_ProjectileReference != NULL )
	{
		AProjectileBase* pTempProjectile = Cast< AProjectileBase >( UGameplayStatics::BeginSpawningActorFromClass( this, this->m_ProjectileReference, FTransform( this->GetPawn()->GetActorRotation(), this->GetPawn()->GetActorLocation() + ( this->GetActorForwardVector() * 100.0f ) ) ) );

		if( pTempProjectile )
		{
			pTempProjectile->Initialize( this, 1500.0f, this->GetCharacteristics()->GetDamage(), 2.0f );

			UGameplayStatics::FinishSpawningActor( pTempProjectile, FTransform( this->GetPawn()->GetActorRotation(), this->GetPawn()->GetActorLocation() + ( this->GetActorForwardVector() * 100.0f ) ) );
		}
	}
}