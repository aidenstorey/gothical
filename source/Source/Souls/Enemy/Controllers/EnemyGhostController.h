// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy/Controllers/EnemyControllerBase.h"
#include "EnemyGhostController.generated.h"

/**
 * 
 */
UCLASS()
class SOULS_API AEnemyGhostController : public AEnemyControllerBase
{
	GENERATED_BODY()
	
	// Member Functions
public:
	AEnemyGhostController( const FObjectInitializer& _krtInitialiser );

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
	FVector m_ChargeDestination;
};
