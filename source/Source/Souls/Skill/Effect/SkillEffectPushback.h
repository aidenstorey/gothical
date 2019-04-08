// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Library Includes

// Local Includes

//Generated Include
#include "SkillEffectPushback.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ASkillEffectPushback : public AActor
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	ASkillEffectPushback( const FObjectInitializer& _krtInitialiser );

	// Other 
	void Initialise( float _fDuration, float _fRadius );

	// UE4 Overrides
	virtual void Tick( float _fDeltaTime ) override;

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() class USphereComponent* m_pPushbackArea;

	UPROPERTY() float m_fDuration;
	UPROPERTY() float m_fTimeElapsed;
	UPROPERTY() float m_fRadius;

};