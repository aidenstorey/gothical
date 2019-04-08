// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Library Includes

// Local Includes

//Generated Include
#include "SkillEffectPoison.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ASkillEffectPoison : public AActor
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	ASkillEffectPoison( const FObjectInitializer& _krtInitialiser );

	// Other
	void Initialise( float _fLifeTime, float _fRadius, float _fPoisonDuration, float _fPoisonRate, int32 _iPoisonAmount );

	// UE4 overrides
	virtual void Tick( float _fDeltaTime ) override;

	// Bound
	UFUNCTION() void OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult );

protected:
private:

	// Member Variables
public:
protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Damage Area" ) ) class USphereComponent* m_pDamageArea;

private:
	UPROPERTY() float m_fLifeTime;
	UPROPERTY() float m_fTimeElapsed;
	UPROPERTY() float m_fRadius;

	UPROPERTY() float m_fPoisonDuration;
	UPROPERTY() float m_fPoisonRate;
	UPROPERTY() int32 m_iPoisonAmount;
};