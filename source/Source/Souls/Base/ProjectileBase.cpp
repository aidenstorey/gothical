// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include "GameFramework/ProjectileMovementComponent.h"

// Library Includes

// Local Includes

// This Include
#include "ProjectileBase.h"

// Implementation

// Constructor
AProjectileBase::AProjectileBase( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
 	// Set this actor to call Tick() every frame
	this->PrimaryActorTick.bCanEverTick = true;
	// Set this actor to collide with other actors
	this->SetActorEnableCollision( true );
	
	// Bind OnHitBehaviour to ActorHit
	this->OnActorHit.AddDynamic( this, &AProjectileBase::OnHitBehaviour );

	// Use ProjectileMovementComponent to determine movement
	this->m_pProjectileMovement = CreateDefaultSubobject< UProjectileMovementComponent >( TEXT( "ProjectileMovement" ) );
	this->m_pProjectileMovement->InitialSpeed				= 0.0f;
	this->m_pProjectileMovement->MaxSpeed					= 0.0f;
	this->m_pProjectileMovement->bRotationFollowsVelocity	= true;
	this->m_pProjectileMovement->bShouldBounce				= false;
	this->m_pProjectileMovement->ProjectileGravityScale		= 0.0f; // No gravity

	// Setup trigger sphere for collision handling
	this->m_TriggerSphere = ObjectInitializer.CreateAbstractDefaultSubobject< USphereComponent >( this, TEXT( "TriggerSphere" ) );
	this->m_TriggerSphere->InitSphereRadius( 10.0f );
	this->m_TriggerSphere->SetCollisionResponseToAllChannels( ECR_Block ); // Block all collisions
	this->m_TriggerSphere->SetCollisionProfileName( FName( "ProjectileEnemy" ) );
	this->RootComponent = this->m_TriggerSphere;
}

void AProjectileBase::Initialize( AController* _pOwner, float _fSpeed, float _fDamage, float _fLifeTime )
{
	this->m_pOwner = _pOwner;
	this->m_pOwnerCharacter = _pOwner->GetCharacter();
	this->m_fDamage = _fDamage;

	// Ensure projectile is spawned parallel to the ground
	FVector vecTemp = CalculateGroundParallel( 1000.0f );

	if( vecTemp != FVector::ZeroVector &&
		CalculateAngleBetweenVectors( this->GetActorForwardVector(), vecTemp ) <= PI / 4.0f )
	{
		this->SetActorRotation( vecTemp.Rotation() );
	}

	this->m_pProjectileMovement->InitialSpeed = _fSpeed;
	this->m_pProjectileMovement->MaxSpeed = _fSpeed;
	this->m_pProjectileMovement->Velocity = this->GetActorForwardVector() * _fSpeed;

	this->m_TriggerSphere->IgnoreActorWhenMoving( this->m_pOwner, true );
	this->m_TriggerSphere->IgnoreActorWhenMoving( this->m_pOwnerCharacter, true );

	this->m_fLifeTime = _fLifeTime;
}

// Called every frame
void AProjectileBase::Tick( float DeltaSeconds )
{
	this->m_fLifeTime -= DeltaSeconds;
	this->ReceiveTick( DeltaSeconds );
	
	FVector vecTemp = CalculateGroundParallel( 1000.0f );

	if( vecTemp != FVector::ZeroVector &&
		CalculateAngleBetweenVectors( this->GetActorForwardVector(), vecTemp ) <= PI / 36.0f )
	{
		this->SetActorRotation( vecTemp.Rotation() );
		this->m_pProjectileMovement->Velocity = this->m_pProjectileMovement->Velocity.Size() * this->GetActorForwardVector();
	}
	
	if( this->m_fLifeTime <= 0.0f )
	{
		this->Destroy();
	}
}

// Function override for derived tick knowledge
void AProjectileBase::ReceiveTick( float _fDeltaTime )
{
	// ...
}

// Accessor Functions
float AProjectileBase::GetDamage()
{
	return( this->m_fDamage );
}

AController* AProjectileBase::GetOwner()
{
	return( this->m_pOwner );
}

UProjectileMovementComponent* AProjectileBase::GetProjectileMovement()
{
	return( this->m_pProjectileMovement );
}

void AProjectileBase::OnHitBehaviour( AActor* _pSelf, AActor* _pHitActor, FVector _NormalImpulse, const FHitResult& _krHit )
{
	if( _pHitActor != nullptr &&
		_pHitActor != this )
	{
		UGameplayStatics::ApplyDamage( _pHitActor, this->m_fDamage, this->m_pOwner->GetInstigatorController(), this->m_pOwnerCharacter, UDamageType::StaticClass() );
		this->Destroy();
	}
}

// Helper Function
// Returns direction vector parallel to ground
FVector AProjectileBase::CalculateGroundParallel( float _fDistanceCheck )
{
	FVector vecReturn = FVector::ZeroVector;
	FHitResult GroundHitResult;

	if( this->GetWorld()->LineTraceSingleByObjectType( GroundHitResult,
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
float AProjectileBase::CalculateAngleBetweenVectors( FVector _vecA, FVector _vecB )
{
	float fReturn;

	//				  ( A . B )
	// cos angle =  -------------
	//				( |A| * |B| )

	fReturn  = FVector::DotProduct( _vecA, _vecB );
	fReturn /= ( _vecA.Size() * _vecB.Size() );
	fReturn  = acos( fReturn );

	return( fReturn );
}