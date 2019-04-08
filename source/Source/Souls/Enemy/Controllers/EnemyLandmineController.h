// Copyright 2015 Half Tower, Inc. All Rights Reserved

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "EnemyControllerBase.h"

// Generated Include
#include "EnemyLandmineController.generated.h"

// Prototypes
class AProjectileBase;

UCLASS( Blueprintable )
class SOULS_API AEnemyLandmineController : public AEnemyControllerBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructor
	AEnemyLandmineController( const FObjectInitializer& _krtInitialiser );

	// Enemy Behaviour
	// Overridden from AEnemyControllerBase
	virtual void ProcessState() override;
	virtual void Attack() override;

protected:
private:

	// Member Variables
public:
	// Projectile reference
	// Used for spawning
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Projectile" )
	TSubclassOf< AProjectileBase > m_ProjectileReference;

protected:
private:

};
