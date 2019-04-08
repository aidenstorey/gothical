// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "Enemy/Controllers/Bosses/BossControllerBase.h"
#include "Enemy/Characters/EnemyCharacter.h"

// Generated Include
#include "BossKnightController.generated.h"

// Prototypes

UCLASS()
class SOULS_API ABossKnightController : public ABossControllerBase
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructor
	ABossKnightController( const FObjectInitializer& _krtInitialiser );

	// Overridden from ABossControllerBase
	virtual void ProcessState() override;
	virtual FVector GetGoToLocation() override;

	// Knight Attacks
	void KnightSlamAttack();

	UFUNCTION( BlueprintCallable, Category = "Knight" )
	void CancelCharge();

	UFUNCTION( Category = "Knight" )
	void OnHealthChange( float _fNewHealth );

	void SpawnWisp();

	virtual void BeginPlay() override;

protected:
	// Other
	virtual void ReceiveDeath() override;

private:

	// Member Variables
public:
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "KnightWisp" )
	TSubclassOf< AEnemyCharacter > m_WispReference;

protected:
private:
	float	m_fWispSpawnRate = 0.1f;
	float	m_fNextWispSpawn = 1.0f - m_fWispSpawnRate;
	int		m_iWispsToSpawn = 3;
};
