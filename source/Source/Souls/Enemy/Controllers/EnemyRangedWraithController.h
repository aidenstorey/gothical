// Copyright 2015 Half Tower, Inc. All Rights Reserved

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "Enemy/Controllers/EnemyRangedController.h"

// Generated Include
#include "EnemyRangedWraithController.generated.h"

// Prototypes

UCLASS( Blueprintable )
class SOULS_API AEnemyRangedWraithController : public AEnemyRangedController
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructor
	AEnemyRangedWraithController( const FObjectInitializer& _krtInitialiser );

	// Enemy Behaviour
	// Overridden from AEnemyRangedController
	virtual void ProcessState() override;
	virtual FVector GetGoToLocation() override;
	virtual void Attack() override;

protected:
private:

	// Member Variables
public:
protected:
private:
};
