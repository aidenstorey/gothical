// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include "Curves/CurveFloat.h"
#include "Engine.h"

// Library Includes

// Local Includes
#include "../Controller/ControllerPlayer.h"

//This Include
#include "CharacterPlayer.h"

// Implementation
ACharacterPlayer::ACharacterPlayer( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	this->GetCapsuleComponent()->SetCollisionProfileName( FName( "Player" ) );

	// Don't rotate character to camera direction
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationRoll = false;

	// Configure character movement
	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->GetCharacterMovement()->bConstrainToPlane = true;
	this->GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom
	this->m_pCameraBoom = _krtInitialiser.CreateDefaultSubobject< USpringArmComponent >( this, TEXT( "m_pCameraBoom" ) );
	this->m_pCameraBoom->AttachTo( this->RootComponent );
	this->m_pCameraBoom->bAbsoluteRotation = true;
	this->m_pCameraBoom->TargetArmLength = 800.0f;
	this->m_pCameraBoom->RelativeRotation = FRotator( -60.0f, 0.0f, 0.0f );
	this->m_pCameraBoom->bDoCollisionTest = false;

	// Create a camera
	this->m_pCamera = _krtInitialiser.CreateDefaultSubobject< UCameraComponent >( this, TEXT( "m_pCamera" ) );
	this->m_pCamera->AttachTo( this->m_pCameraBoom, USpringArmComponent::SocketName );
	this->m_pCamera->bUsePawnControlRotation = false;

	// Create pickup area collider
	this->m_pPickupArea = _krtInitialiser.CreateDefaultSubobject< UCapsuleComponent >( this, "PickupArea" );
	this->m_pPickupArea->SetCapsuleSize( 60.0f, 96.0f );
	this->m_pPickupArea->SetCollisionProfileName( FName( "PlayerInteractArea" ) );
}

UCameraComponent* ACharacterPlayer::GetCamera() const
{
	return ( this->m_pCamera );
}

USpringArmComponent* ACharacterPlayer::GetCameraBoom() const
{
	return ( this->m_pCameraBoom );
}

UBoxComponent* ACharacterPlayer::GetRotationCollider() const
{
	return ( this->m_pRotationCollider );
}

UCapsuleComponent* ACharacterPlayer::GetPickupArea() const
{
	return ( this->m_pPickupArea );
}

void ACharacterPlayer::BeginRegenParticles()
{
	// Check if there are attached regen particles and they aren't currently running
		// True - spawn a new particle emitter on ground socket
	if ( this->m_pRegenParticles != nullptr && this->m_pActiveParticles == nullptr )
	{
		this->m_pActiveParticles = UGameplayStatics::SpawnEmitterAttached( this->m_pRegenParticles,
																		   this->GetMesh(),
																		   FName( "ground_socket" ) );
	}
}

void ACharacterPlayer::EndRegenParticles()
{
	// Check if there is an active particles
		// True - destroy the particles
	if ( this->m_pActiveParticles != nullptr )
	{
		this->m_pActiveParticles->DestroyComponent( true );
		this->m_pActiveParticles = nullptr;
	}
}

float ACharacterPlayer::TakeDamage( float _fDamageAmount, struct FDamageEvent const& _krtDamageEvent, class AController* _pEventInstigator, AActor* _pDamageCauser )
{
	Super::TakeDamage( _fDamageAmount, _krtDamageEvent, _pEventInstigator, _pDamageCauser );

	AControllerPlayer* pController{ Cast< AControllerPlayer >( this->GetController() ) };
	if ( pController != nullptr )
	{
		pController->PlayerTakeDamage( static_cast< int >( _fDamageAmount ) );
	}
	
	return ( _fDamageAmount );
}