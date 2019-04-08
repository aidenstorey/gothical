// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Library Includes

// Local Includes

//Generated Include
#include "SkillEffectFreeze.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ASkillEffectFreeze : public AActor
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors 
	ASkillEffectFreeze( const FObjectInitializer& _krtInitialiser );

	// Other 
	void Initialise( float _fStunDuration, float _fDuration, float _fSpeed, float _fAngle );

	// UE4 Overrides
	virtual void Tick( float _fDeltaTime ) override;

	// Bound
	UFUNCTION() void OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult );

protected:
private:
	FVector CalculateGroundParallel( float _fDistanceCheck );
	float CalculateAngleBetweenVectors( FVector _vecA, FVector _vecB );

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls" ) class UBoxComponent* m_pFreezeArea;

protected:
private:
	UPROPERTY()	UProjectileMovementComponent*	m_pProjectileMovement;
	UPROPERTY() FVector m_vStartLocation;
	UPROPERTY() float m_fStunDuration;
	UPROPERTY() float m_fDuration;
	UPROPERTY() float m_fAngle;

};