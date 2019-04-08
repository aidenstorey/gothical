// Copyright 2015 Half Tower, Inc. All Rights Reserved

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Library Includes

// Local Includes

// Generated Include
#include "ProjectileBase.generated.h"

// Prototypes
class UProjectileMovementComponent;

UCLASS()
class SOULS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
	// Member Functions
public:	
	// Constructor
	AProjectileBase( const FObjectInitializer& ObjectInitializer );

	// Sets initial values
	void Initialize( AController* _pOwner, float _fSpeed, float _fDamage, float _fLifeTime );

	// UE4 Overrides
	virtual void Tick( float DeltaSeconds ) override;

	// Accessor Functions
	float							GetDamage();
	AController*					GetOwner();
	UProjectileMovementComponent*	GetProjectileMovement();
	
	// Bound
	UFUNCTION()
	void OnHitBehaviour( AActor* _pSelf, AActor* _pHitActor, FVector _NormalImpulse, const FHitResult& _krHit );

protected:
	virtual void ReceiveTick( float _fDeltaTime );

private:
	FVector CalculateGroundParallel( float _fDistanceCheck );
	float CalculateAngleBetweenVectors( FVector _vecA, FVector _vecB );

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Movement, meta = ( AllowPrivateAccess = "true" ) )
	UProjectileMovementComponent*	m_pProjectileMovement;

	UPROPERTY( EditAnywhere, Category = "Projectile Componenets" )
	class USphereComponent* m_TriggerSphere;

protected:
	UPROPERTY() AController*	m_pOwner;
	UPROPERTY() ACharacter*		m_pOwnerCharacter;
	UPROPERTY() float			m_fDamage;
	UPROPERTY() float			m_fLifeTime;


private:
};
