// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Controllers/EnemyControllerBase.h"
#include "../../QuarrieDamageBPLibrary.h"
#include "../../Player/Character/CharacterPlayer.h"

// This Include
#include "BomberCharacter.h"

// Implementation

// Constructor
ABomberCharacter::ABomberCharacter( const FObjectInitializer& ObjectInitializer ) :
Super( ObjectInitializer )
{
	this->TriggerSphere = ObjectInitializer.CreateAbstractDefaultSubobject< USphereComponent >( this, TEXT( "TriggerSphere" ) );
	this->TriggerSphere->InitSphereRadius( 150.0f );
	this->TriggerSphere->AttachParent = this->RootComponent;
	this->TriggerSphere->SetCollisionProfileName( FName( "EnemyGhost" ) );

	this->TriggerSphere->OnComponentBeginOverlap.AddDynamic( this, &ABomberCharacter::OnOverlapBegin );
}

float ABomberCharacter::TakeDamage( float _fDamageAmount, struct FDamageEvent const& _krtDamageEvent, class AController* _pEventInstigator, AActor* _pDamageCauser )
{
	AEnemyControllerBase* pController{ Cast< AEnemyControllerBase >( this->GetController() ) };
	AEnemyCharacter* pCharacterTemp = Cast< AEnemyCharacter >( _pDamageCauser );

	if( pController != nullptr &&
	    !pCharacterTemp )
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

// Enemy behaviour
// Explode - Cause radial damage
void ABomberCharacter::Explode()
{
	// TODO - Ethan : Magic Numbers
	UQuarrieDamageBPLibrary::QApplyRadialDamage( this->GetWorld(), 10.0f, this->GetActorLocation(), 300.0f, UDamageType::StaticClass(), TArray< AActor* >(), false, this );
	this->Destroy();
}

// Trigger explosion begin
void ABomberCharacter::OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult )
{
	// TODO - Ethan : Magic Numbers
	AEnemyControllerBase* pTemp = Cast< AEnemyControllerBase >( this->GetController() );

	if( pTemp != nullptr &&
		Cast< ACharacterPlayer >( _OtherActor ) != nullptr &&
		!_OtherComp->ComponentHasTag( FName( "IgnoreDamage" ) ) )
	{
		if( pTemp->GetCurrentState() != EEnemyState::ENEMY_STATE_PRE_EXPLODE &&
			pTemp->GetCurrentState() != EEnemyState::ENEMY_STATE_EXPLODE )
		{
			pTemp->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_EXPLODE );
			pTemp->SetCurrentCooldown( 1.5f );
		}
	}
}