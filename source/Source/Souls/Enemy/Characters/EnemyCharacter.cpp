// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include "Curves/CurveFloat.h"
#include "Engine.h"
#include "SkeletalMeshTypes.h"

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../Controllers/EnemyControllerBase.h"
#include "../Controllers/Bosses/BossControllerBase.h"
#include "../../World/DamageIndicator.h"
#include "../../Player/Other/ProjectilePlayer.h"

//This Include
#include "EnemyCharacter.h"

// Implementation
AEnemyCharacter::AEnemyCharacter( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	PrimaryActorTick.bCanEverTick = true;

	this->GetCapsuleComponent()->SetCollisionProfileName( FName( "Enemy" ) );
}

float AEnemyCharacter::TakeDamage( float _fDamageAmount, struct FDamageEvent const& _krtDamageEvent, class AController* _pEventInstigator, AActor* _pDamageCauser )
{
	AEnemyControllerBase* pController{ Cast< AEnemyControllerBase >( this->GetController() ) };
	AEnemyCharacter* pCharacterTemp = Cast< AEnemyCharacter >( _pDamageCauser );

	if ( pController != nullptr &&
		 ( !pCharacterTemp ||
		   Cast< ABossControllerBase >( pCharacterTemp->GetController() ) ) )
	{
		// Do this before taking damage in case enemy is destroyed
		if( pController->GetCurrentState() == EEnemyState::ENEMY_STATE_IDLE )
		{
			pController->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
			pController->AlertOtherEnemies();
		}

		pController->GetCharacteristics()->TakeDamage( static_cast< int >( _fDamageAmount ), _pDamageCauser );
		this->StartHit();

		// FActorSpawnParameters SpawnParams;
		//
		// this->GetWorld()->SpawnActor< ADamageIndicator >( this->GetActorLocation() + FVector( 0.0f, 0.0f, 100.0f ), this->GetActorRotation(), SpawnParams )->Initialise( _fDamageAmount );
	}

	return ( _fDamageAmount );
}

void AEnemyCharacter::SetAllTicks( bool _NewTick )
{
	for( UActorComponent* Child : GetComponents() )
	{
		if( Child->GetClass() == UCharacterMovementComponent::StaticClass() )
		{
			Cast< UCharacterMovementComponent >( Child )->StopMovementImmediately();
		}
		else if( Child->GetClass() != USkeletalMeshComponent::StaticClass() )
		{
			Child->SetComponentTickEnabled( _NewTick );
			// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Green, "Totes" );
		}
	}
}

void AEnemyCharacter::ToggleAllTicks()
{
	for( UActorComponent* Child : GetComponents() )
	{
		if( Child->GetClass() == UCharacterMovementComponent::StaticClass() )
		{
			Cast< UCharacterMovementComponent >( Child )->StopMovementImmediately();
		}
		else if( Child->GetClass() != USkeletalMeshComponent::StaticClass() )
		{
			Child->SetComponentTickEnabled( !Child->IsComponentTickEnabled() );
		}
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( this->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) > 1600.0f )
	{
		this->ToggleAllTicks();
	}
	
	if( this->GetController() != nullptr )
	{
		this->GetController()->BeginPlay();
		this->GetCharacterMovement()->MaxWalkSpeed = Cast< AEnemyControllerBase >( this->GetController() )->GetCharacteristics()->GetMovementSpeed();
	}
}

void AEnemyCharacter::AddSeeker( AProjectilePlayer* _pNewSeeker )
{
	m_pCurrentSeekers.Add( _pNewSeeker );
}

void AEnemyCharacter::StopSeekers()
{
	for( auto Iter( m_pCurrentSeekers.CreateIterator() ); Iter; Iter++ )
	{
		( *Iter )->StopHoming();
	}
}

void AEnemyCharacter::BeginPoisonParticles()
{
	// Check if there are attached regen particles and they aren't currently running
		// True - spawn a new particle emitter on ground socket
	if ( this->m_pPoisonParticles != nullptr && this->m_pActiveParticles == nullptr )
	{
		this->m_pActiveParticles = UGameplayStatics::SpawnEmitterAttached( this->m_pPoisonParticles,
																		   this->GetMesh(),
																		   FName( "ground_socket" ) );
	}

	this->StartPoison();
}

void AEnemyCharacter::EndPoisonParticles()
{
	// Check if there is an active particles
		// True - destroy the particles
	if ( this->m_pActiveParticles != nullptr )
	{
		this->m_pActiveParticles->DestroyComponent( true );
		this->m_pActiveParticles = nullptr;
	}

	this->EndPoison();
}

void AEnemyCharacter::FreezeCharacter()
{
	this->FreezeAnimation();

	this->StartFreeze();
}

void AEnemyCharacter::UnfreezeCharacter()
{
	this->UnfreezeAnimation();

	this->EndFreeze();
}

void AEnemyCharacter::FreezeAnimation()
{
	for( UActorComponent* Child : GetComponents() )
	{
		if( Child->GetClass() == USkeletalMeshComponent::StaticClass() )
		{
			Child->SetComponentTickEnabled( false );
		}
	}
}

void AEnemyCharacter::UnfreezeAnimation()
{
	for( UActorComponent* Child : GetComponents() )
	{
		if( Child->GetClass() == USkeletalMeshComponent::StaticClass() )
		{
			Child->SetComponentTickEnabled( true );
		}
	}
}