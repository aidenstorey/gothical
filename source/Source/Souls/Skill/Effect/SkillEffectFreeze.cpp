// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Enemy/Characters/EnemyCharacter.h"
#include "../../Enemy/Controllers/EnemyControllerBase.h"

//This Include
#include "SkillEffectFreeze.h"

// Implementation
ASkillEffectFreeze::ASkillEffectFreeze( const FObjectInitializer& _krtInitialiser )
{
	// Set this actor to call Tick() every frame
	this->PrimaryActorTick.bCanEverTick = true;

	this->m_pProjectileMovement = CreateDefaultSubobject< UProjectileMovementComponent >( TEXT( "ProjectileMovement" ) );
	this->m_pProjectileMovement->InitialSpeed = 0.0f;
	this->m_pProjectileMovement->MaxSpeed = 0.0f;
	this->m_pProjectileMovement->bRotationFollowsVelocity = true;
	this->m_pProjectileMovement->bShouldBounce = false;
	this->m_pProjectileMovement->ProjectileGravityScale = 0.0f; // No gravity

	this->m_pFreezeArea = _krtInitialiser.CreateDefaultSubobject< UBoxComponent >( this, "FreezeArea" );
	this->RootComponent = this->m_pFreezeArea;
	this->m_pFreezeArea->SetCollisionProfileName( FName( "FreezeArea" ) );
	this->m_pFreezeArea->InitBoxExtent( { 20.0f, 20.0f, 20.0f } );
	this->m_pFreezeArea->OnComponentBeginOverlap.AddDynamic( this, &ASkillEffectFreeze::OnOverlapBegin );
}

void ASkillEffectFreeze::Initialise( float _fStunDuration, float _fDuration, float _fSpeed, float _fAngle )
{
	this->m_vStartLocation = this->GetActorLocation();
	this->m_fStunDuration = _fStunDuration;
	this->m_fDuration = _fDuration;
	this->m_fAngle = _fAngle;

	// Ensure projectile is spawned parallel to the ground
	FVector vecTemp = CalculateGroundParallel( 1000.0f );

	if ( vecTemp != FVector::ZeroVector &&
		 CalculateAngleBetweenVectors( this->GetActorForwardVector(), vecTemp ) <= PI / 4.0f )
	{
		this->SetActorRotation( vecTemp.Rotation() );
	}

	this->m_pProjectileMovement->InitialSpeed = _fSpeed;
	this->m_pProjectileMovement->MaxSpeed = _fSpeed;
	this->m_pProjectileMovement->Velocity = this->GetActorForwardVector() * _fSpeed;
}

void ASkillEffectFreeze::Tick( float _fDeltaTime )
{
	this->m_fDuration -= _fDeltaTime;

	FVector vecTemp = this->CalculateGroundParallel( 1000.0f );

	if ( vecTemp != FVector::ZeroVector &&
		 this->CalculateAngleBetweenVectors( this->GetActorForwardVector(), vecTemp ) <= PI / 36.0f )
	{
		this->SetActorRotation( vecTemp.Rotation() );
		this->m_pProjectileMovement->Velocity = this->m_pProjectileMovement->Velocity.Size() * this->GetActorForwardVector();
	}

	FVector vDistance{ this->GetActorLocation() - this->m_vStartLocation };
	float fSize{ tanf( FMath::DegreesToRadians( this->m_fAngle ) ) * vDistance.Size() };
	this->m_pFreezeArea->SetBoxExtent( FVector{ 20.0f, fSize, 20.0f } );

	if ( this->m_fDuration <= 0.0f )
	{
		this->Destroy();
	}
}

// Helper Function
// Returns direction vector parallel to ground
FVector ASkillEffectFreeze::CalculateGroundParallel( float _fDistanceCheck )
{
	FVector vecReturn = FVector::ZeroVector;
	FHitResult GroundHitResult;

	if ( this->GetWorld()->LineTraceSingleByObjectType( GroundHitResult,
		this->GetActorLocation(),
		this->GetActorLocation() - FVector( 0.0f, 0.0f, _fDistanceCheck ),
		FCollisionObjectQueryParams( ECC_TO_BITFIELD( ECC_WorldStatic ) ) ) )
	{
		// Normal of the ground rotated relative to the object
		vecReturn = GroundHitResult.ImpactNormal.RotateAngleAxis( 90.0f, this->GetActorRightVector() );
		// Collapses vector to remove horizontal variation
		vecReturn = FVector::PointPlaneProject( vecReturn, FVector( 0.0f, 0.0f, 0.0f ), this->GetActorRightVector() );

		vecReturn.Normalize();
	}

	return( vecReturn );
}

// Helper Function
// Returns the radian angle between two vectors in 3D
float ASkillEffectFreeze::CalculateAngleBetweenVectors( FVector _vecA, FVector _vecB )
{
	float fReturn;

	//				  ( A . B )
	// cos angle =  -------------
	//				( |A| * |B| )

	fReturn = FVector::DotProduct( _vecA, _vecB );
	fReturn /= ( _vecA.Size() * _vecB.Size() );
	fReturn = acos( fReturn );

	return( fReturn );
}

void ASkillEffectFreeze::OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult )
{
	auto pEnemy = Cast< AEnemyControllerBase >( Cast< AEnemyCharacter >( _OtherActor )->GetController() );
	if ( pEnemy != nullptr )
	{
		pEnemy->FreezeEnemy( this->m_fStunDuration );
	}
}