// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Incldues

// Local Includes
#include "../../../QuarrieDamageBPLibrary.h"

// This Includes
#include "BossReaperController.h"

// Implementation

// Constructor

ABossReaperController::ABossReaperController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	// TODO - Ethan : Data drive this better
	this->SetName( TEXT( "Reaper" ) );

	// ACharacter* pTemp = Cast< ACharacter >( this->GetPawn() );
}

void ABossReaperController::ProcessState()
{
	if( this->GetCurrentCooldown() <= 0.0f )
	{
		switch( this->GetCurrentBossState() )
		{
			case EBossState::BOSS_STATE_IDLE:
			{
				if( this->DetectPlayer() )
				{
					this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
					this->SetCurrentCooldown( 3.0f );
				}
			}	break;

			case EBossState::BOSS_STATE_WANDER:
			{

			}	break;

			case EBossState::BOSS_STATE_SEEKING:
			{
				int iTemp = FMath::RandRange( 0, 9 );

				if( iTemp <= 5 &&
					this->GetBossCooldown( 0 ) <= 0.0f &&
					this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) <= 300.0f )
				{
					this->SetCurrentBossState( EBossState::BOSS_STATE_PRE_ATTACK_A );
					this->SetCurrentCooldown( 0.623f );
				}
				else if( iTemp <= 8 &&
						 this->GetBossCooldown( 1 ) <= 0.0f &&
						 this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) <= 300.0f )
				{
					this->SetCurrentBossState( EBossState::BOSS_STATE_PRE_ATTACK_B );
					this->SetCurrentCooldown( 0.951f );
				}
				else if( this->GetBossCooldown( 2 ) <= 0.0f )
				{
					this->GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
					this->SetCurrentBossState( EBossState::BOSS_STATE_PRE_ATTACK_C );
					this->SetCurrentCooldown( 6.5f );
				}
				else
				{
					this->SetCurrentCooldown( 1.0f );
				}
			}	break;

			case EBossState::BOSS_STATE_FLEEING:
			{

			}	break;

			// Reaper - Attack A - Reap
			case EBossState::BOSS_STATE_PRE_ATTACK_A:
			{ // 0.623
				this->Reap();
				this->SetCurrentBossState( EBossState::BOSS_STATE_ATTACK_A );
				this->SetCurrentCooldown( 0.688f );
			}	break;

			case EBossState::BOSS_STATE_ATTACK_A:
			{
				this->Reap();
				this->SetCurrentBossState( EBossState::BOSS_STATE_POST_ATTACK_A );
				this->SetCurrentCooldown( 0.689f );
			}	break;

			case EBossState::BOSS_STATE_POST_ATTACK_A:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
				this->SetBossCooldown( 0, 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			// Reaper - Attack B - Rend
			case EBossState::BOSS_STATE_PRE_ATTACK_B:
			{
				this->Rend();
				this->SetCurrentBossState( EBossState::BOSS_STATE_ATTACK_B );
				this->SetCurrentCooldown( 0.524f );
				this->SetBossCooldown( 0, 7.0f );
			}	break;

			case EBossState::BOSS_STATE_ATTACK_B:
			{
				this->Rend();
				this->SetCurrentBossState( EBossState::BOSS_STATE_POST_ATTACK_B );
				this->SetCurrentCooldown( 0.525f );
			}	break;

			case EBossState::BOSS_STATE_POST_ATTACK_B:

			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			// Reaper - Attack C - Harvest
			case EBossState::BOSS_STATE_PRE_ATTACK_C:
			{

				this->Harvest();
				this->SetCurrentBossState( EBossState::BOSS_STATE_ATTACK_C );
				this->SetCurrentCooldown( 0.5f );
			}	break;

			case EBossState::BOSS_STATE_ATTACK_C:
			{
				this->Harvest();
				this->SetCurrentBossState( EBossState::BOSS_STATE_POST_ATTACK_C );
				this->SetCurrentCooldown( 1.0f );
			}	break;

			case EBossState::BOSS_STATE_POST_ATTACK_C:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			case EBossState::BOSS_STATE_PRE_HIT:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_HIT );
				this->SetCurrentCooldown( this->m_fHitCooldown );
			}	break;

			case EBossState::BOSS_STATE_HIT:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_POST_HIT );
			}	break;

			case EBossState::BOSS_STATE_POST_HIT:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
				this->m_fHitCooldown = 5.0f;
			}	break;

			default: break;
		}
	}

	if( this->GetCurrentBossState() == EBossState::BOSS_STATE_SEEKING )
	{
		this->MoveToLocation( this->GetGoToLocation() );

		// FVector Destination = this->GetGoToLocation();
		// 
		// this->GetPawn()->AddMovementInput( Destination - this->GetPawn()->GetActorLocation() );
		// 
		// FVector tLookAt{ Destination - this->GetPawn()->GetActorLocation() };
		// FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };
		// 
		// this->GetPawn()->SetActorRotation( tLookAtRotation );
	}

	if( this->GetCurrentBossState() == EBossState::BOSS_STATE_PRE_ATTACK_C )
	{
		this->Gravitate();
	}

	
}

FVector ABossReaperController::GetGoToLocation()
{
	// int iTemp = FMath::RandRange( 0, 1 );
	// 
	// if( iTemp )
	// {
	// 	FVector vecReturn = this->GetPawn()->GetActorLocation() - UGameplayStatics::GetPlayerCharacter( this, 0 )->GetActorLocation();
	// 	vecReturn = vecReturn.RotateAngleAxis( 20.0f, FVector( 0.0f, 0.0f, 1.0f ) );
	// 	return( UGameplayStatics::GetPlayerCharacter( this, 0 )->GetActorLocation() + vecReturn );
	// }
	// else
	{
		return( UGameplayStatics::GetPlayerCharacter( this, 0 )->GetActorLocation() );
	}
}

void ABossReaperController::Reap()
{
	UQuarrieDamageBPLibrary::QApplyConicDamage( this,
												10.0f, // TODO - Ethan : Actual Damage
												this->GetPawn()->GetActorLocation(),
												400.0f,
												75.0f,
												this->GetPawn()->GetActorForwardVector(),
												UDamageType::StaticClass(),
												TArray< AActor* >(),
												false,
												this->GetPawn(),
												this,
												true );
}

void ABossReaperController::Rend()
{
	UQuarrieDamageBPLibrary::QApplyRadialDamage( this,
												 10.0f, // TODO - Ethan : Actual Damage
												 this->GetPawn()->GetActorLocation() + ( this->GetPawn()->GetActorForwardVector() * 300.0f ) + FVector( 0.0f, 0.0f, -150.0f ),
												 150.0f,
												 UDamageType::StaticClass(),
												 TArray< AActor* >(),
												 false,
												 this->GetPawn(),
												 this,
												 true );
}

void ABossReaperController::Gravitate()
{
	TArray< FOverlapResult > OverlapList;

	this->GetWorld()->OverlapMultiByObjectType( OverlapList,
												this->GetPawn()->GetActorLocation(),
												FQuat::Identity,
												FCollisionObjectQueryParams( ECC_TO_BITFIELD( ECC_GameTraceChannel2 ) &	 // Player Collision Channel
																			 ECC_TO_BITFIELD( ECC_GameTraceChannel3 ) &  // Enemy Collision Channel
																			 ECC_TO_BITFIELD( ECC_GameTraceChannel6 ) ), // Ghost Collision Channel
												FCollisionShape::MakeSphere( 2000.0f ) );

	for( int iIndex{ 0 }; iIndex < OverlapList.Num(); ++iIndex )
	{
		ACharacter* pActor{ Cast< ACharacter >( OverlapList[ iIndex ].GetActor() ) };

		if( pActor )
		{
			FVector ForceDirection = ( this->GetPawn()->GetActorLocation() - pActor->GetActorLocation() );
			ForceDirection.Z = 0.0f;
			ForceDirection.Normalize();
			pActor->GetCharacterMovement()->AddImpulse( ForceDirection * this->m_fDragStrength * ( 1.0 - ( this->GetPawn()->GetDistanceTo( pActor ) / 2000.0f ) ), true );
		}
	}
}

void ABossReaperController::Harvest()
{
	UQuarrieDamageBPLibrary::QApplyRadialDamage( this,
												 10.0f, // TODO - Ethan : Actual Damage
												 this->GetPawn()->GetActorLocation(),
												 350.0f,
												 UDamageType::StaticClass(),
												 TArray< AActor* >(),
												 false,
												 this->GetPawn(),
												 this,
												 true );
}

void ABossReaperController::ReceiveDeath()
{
	// Attempt to get the current gamemode
	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		// Set the reaper boss win condition as met
		pGameMode->WinConditionMet( EWinCondition::WC_CATHEDERAL_GUARDIAN );
	}
}