// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../../Attribute/Characteristics.h"
#include "../../../QuarrieDamageBPLibrary.h"
#include "../../../SoulsGameMode.h"

// This Includes
#include "BossKnightController.h"

// Implementation

// Constructor
ABossKnightController::ABossKnightController( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	// TODO - Ethan : Data drive some stuff

	this->SetName( TEXT( "Knight" ) );
}

void ABossKnightController::BeginPlay()
{
	Super::BeginPlay();

	this->GetCharacteristics()->GetOnHealthChangeDelegate().AddDynamic( this, &ABossKnightController::OnHealthChange );
}

void ABossKnightController::ProcessState()
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
				}
			}	break;

			case EBossState::BOSS_STATE_WANDER:
			{

			}	break;

			case EBossState::BOSS_STATE_SEEKING:
			{
				if( this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) <= 300.0f &&
					this->GetBossCooldown( 0 ) <= 0.0f )
				{
					this->SetCurrentBossState( EBossState::BOSS_STATE_PRE_ATTACK_A );
					this->SetCurrentCooldown( 1.32f );
				}
				else if( this->GetBossCooldown( 1 ) <= 0.0f )
				{
					this->SetCurrentBossState( EBossState::BOSS_STATE_PRE_ATTACK_B );
					this->SetCurrentCooldown( 3.3f );
				}

			}	break;

			case EBossState::BOSS_STATE_FLEEING:
			{

			}	break;

			// Knight - Attack A - Slam
			case EBossState::BOSS_STATE_PRE_ATTACK_A:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_ATTACK_A );
			}	break;

			case EBossState::BOSS_STATE_ATTACK_A:
			{
				this->KnightSlamAttack();
				this->SetCurrentBossState( EBossState::BOSS_STATE_POST_ATTACK_A );
				this->SetCurrentCooldown( 1.98f );
			}	break;

			case EBossState::BOSS_STATE_POST_ATTACK_A:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
				this->SetCurrentCooldown( 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
				this->SetBossCooldown( 0, 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			// Knight - Attack B - Charge
			case EBossState::BOSS_STATE_PRE_ATTACK_B:
			{
				this->MoveTo( this->GetPawn()->GetActorLocation() );
				this->SetCurrentBossState( EBossState::BOSS_STATE_ATTACK_B );
				this->SetCurrentCooldown( 2.0f );
			}	break;

			case EBossState::BOSS_STATE_ATTACK_B:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_POST_ATTACK_B );
			}	break;

			case EBossState::BOSS_STATE_POST_ATTACK_B:
			{
				this->SetCurrentBossState( EBossState::BOSS_STATE_SEEKING );
				this->SetBossCooldown( 1, 8.0f ); // 10.0f / this->GetCharacteristics()->GetAttackSpeed() );
			}	break;

			// Knight - Attack C - Currently unused
			case EBossState::BOSS_STATE_PRE_ATTACK_C:
			{

			}	break;

			case EBossState::BOSS_STATE_ATTACK_C:
			{

			}	break;

			case EBossState::BOSS_STATE_POST_ATTACK_C:
			{

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

	if( this->GetCurrentBossState() == EBossState::BOSS_STATE_SEEKING
		// || this->GetCurrentBossState() == EBossState::BOSS_STATE_ATTACK_B
		)
	{
		this->MoveTo( this->GetGoToLocation() );
	}

	if( this->GetCurrentBossState() == EBossState::BOSS_STATE_ATTACK_B )
	{
		this->GetCharacter()->AddMovementInput( this->GetPawn()->GetActorForwardVector() );
	}

	if( this->GetCurrentBossState() == EBossState::BOSS_STATE_PRE_ATTACK_B )
	{
		FVector tLookAt{ UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() - this->GetPawn()->GetActorLocation() };
		FRotator tLookAtRotation{ 0.0f, tLookAt.Rotation().Yaw, 0.0f };

		this->GetPawn()->SetActorRotation( tLookAtRotation );
	}
}

FVector ABossKnightController::GetGoToLocation()
{
	// if( this->GetCurrentBossState() == EBossState::BOSS_STATE_ATTACK_B )
	// {
	// 	 return( this->GetPawn()->GetActorLocation() + ( this->GetPawn()->GetActorForwardVector() * 50.0f ) );
	// }
	// else
	{
		return( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 )->GetActorLocation() );
	}
}

void ABossKnightController::KnightSlamAttack()
{
	// UGameplayStatics::ApplyRadialDamage( this->GetWorld(), 10.0f /* TODO - Ethan : Replace with actual Damage */, this->GetPawn()->GetActorLocation() + ( this->GetPawn()->GetActorForwardVector() * 400.0f ) + FVector( 0.0f, 0.0f, -150.0f ), 60.0f, UDamageType::StaticClass(), TArray< AActor* >(), this->GetPawn(), this, true );
	UQuarrieDamageBPLibrary::QApplyRadialDamage( this->GetWorld(), 10.0f /* TODO - Ethan : Replace with actual Damage */, this->GetPawn()->GetActorLocation() + ( this->GetPawn()->GetActorForwardVector() * 400.0f ) + FVector( 0.0f, 0.0f, -150.0f ), 150.0f, UDamageType::StaticClass(), TArray< AActor* >(), false, this->GetPawn(), this, true );
}

void ABossKnightController::ReceiveDeath()
{
	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		pGameMode->WinConditionMet( EWinCondition::WC_GREAT_KNIGHT );
	}
}

void ABossKnightController::CancelCharge()
{
	this->SetCurrentBossState( EBossState::BOSS_STATE_POST_ATTACK_B );
}

void ABossKnightController::OnHealthChange( float _fNewHealth )
{
	if( _fNewHealth < m_fNextWispSpawn )
	{
		m_fNextWispSpawn -= m_fWispSpawnRate;

		for( int iIndex = 0; iIndex < m_iWispsToSpawn; iIndex++ )
		{
			SpawnWisp();
		}

		
	}
}

void ABossKnightController::SpawnWisp()
{
	// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Magenta, "SpawnFunction" );

	if( this->m_WispReference != nullptr )
	{
		AEnemyCharacter* pTempWisp = Cast< AEnemyCharacter >( UGameplayStatics::BeginSpawningActorFromClass( this, this->m_WispReference, FTransform( this->GetCharacter()->GetActorRotation(), this->GetCharacter()->GetActorLocation() + FVector( FMath::FRandRange( 100.0f, 150.0f ), 0.0f, 0.0f ).RotateAngleAxis( FMath::FRandRange( 0.0f, 360.0f ), FVector( 0.0f, 0.0f, 1.0f ) ) ) ) );

		// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Blue, "SpawnBegin" );

		if( pTempWisp )
		{
			// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Green, "SpawnFinish" );
			UGameplayStatics::FinishSpawningActor( pTempWisp, FTransform( this->GetCharacter()->GetActorRotation(), this->GetCharacter()->GetActorLocation() + FVector( FMath::FRandRange( 100.0f, 150.0f ), 0.0f, 0.0f ).RotateAngleAxis( FMath::FRandRange( 0.0f, 360.0f ), FVector( 0.0f, 0.0f, 1.0f ) ) ) );
		}
	}
}