// Copyright 2015 Half Tower, Inc. All Rights Reserved

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "EnemyControllerBase.h"

// Generated Include
#include "EnemyWispController.generated.h"

// Prototypes

UCLASS( Blueprintable )
class SOULS_API AEnemyWispController : public AEnemyControllerBase
{
	GENERATED_BODY()

		// Member Functions
public:
	// Constructor
	AEnemyWispController( const FObjectInitializer& _krtInitialiser );

	// Enemy Behaviour
	// Overridden from AEnemyControllerBase
	virtual void ProcessState() override;
	virtual FVector GetGoToLocation() override;

protected:
private:

	// Member Variables
public:
protected:
private:

};
