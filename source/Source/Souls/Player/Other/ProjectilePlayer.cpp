// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Enemy/Characters/EnemyCharacter.h"

//This Include
#include "ProjectilePlayer.h"

// Implementation
AProjectilePlayer::AProjectilePlayer( const FObjectInitializer& _rtInitialiser ) :
Super( _rtInitialiser )
{
	// Create base homing trigger
	this->m_pHomingTrigger = _rtInitialiser.CreateDefaultSubobject< USphereComponent >( this, TEXT( "HomingTrigger" ) );
	this->m_pHomingTrigger->AttachTo( this->RootComponent );
	this->m_pHomingTrigger->SetCollisionProfileName( FName( "ProjectilePlayer" ) );
	this->m_TriggerSphere->SetCollisionProfileName( FName( "ProjectileBase" ) );

	this->m_pHomingTrigger->OnComponentBeginOverlap.AddDynamic( this, &AProjectilePlayer::OnOverlapBegin );

	this->m_pHomingTrigger->AddLocalOffset( FVector( 300.0f, 0.0f, 0.0f ) );
}

void AProjectilePlayer::SetHomingDistance( float _fDistance )
{
	this->m_fHomingDistance = _fDistance;
	this->m_pHomingTrigger->SetSphereRadius( _fDistance );
}

void AProjectilePlayer::ReceiveTick( float _fDeltaTime )
{
	if( m_pHomingTarget != nullptr )
	{
		// GEngine->AddOnScreenDebugMessage( -1, 60.0f, FColor::Red, ( ( m_pHomingTarget->GetActorLocation() - this->GetActorLocation() ).Rotation() - this->GetActorRotation() ).ToString() );

		FRotator rotTemp = ( ( ( m_pHomingTarget->GetActorLocation() - this->GetActorLocation() ).Rotation() - this->GetActorRotation() ) );

		if( abs( rotTemp.Yaw ) > 180.0f )
		{
			rotTemp.Yaw += ( rotTemp.Yaw < 0.0f ? 360.0f : -360.0f );
		}

		this->SetActorRotation( this->GetActorRotation() + ( rotTemp * 10.0f * _fDeltaTime ) );
		this->m_pProjectileMovement->Velocity = this->m_pProjectileMovement->Velocity.Size() * this->GetActorForwardVector();
	}
}

void AProjectilePlayer::OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult )
{
	if( m_pHomingTarget == nullptr )
	{
		// If other actor is not an enemy, m_pHomingTarget will remain NULL
		m_pHomingTarget = Cast< AEnemyCharacter >( _OtherActor );

		if( m_pHomingTarget != nullptr )
		{
			m_pHomingTarget->AddSeeker( this );
		}
	}
}

void AProjectilePlayer::StopHoming()
{
	m_pHomingTarget = nullptr;
}