// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "EnemyControllerBase.h"

// Generated Include
#include "EnemyMeleeController.generated.h"

// Prototypes


UCLASS( Blueprintable )
class AEnemyMeleeController : public AEnemyControllerBase
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructor
	AEnemyMeleeController( const FObjectInitializer& _krtInitialiser );

	// Enemy Behaviour
	// Overridden from AEnemyControllerBase
	virtual void ProcessState() override;
	virtual FVector GetGoToLocation() override;
	virtual void Attack() override;

protected:
private:

	// Member Variables
public:
	FVector m_SeekDisplacement;
protected:
private:
};
