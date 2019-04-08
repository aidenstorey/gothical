// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Library Includes

// Local Includes

// Generated Include
#include "AreaLock.generated.h"

// Prototypes

UCLASS()
class SOULS_API AAreaLock : public AActor
{
	GENERATED_BODY()
	
	// Member Functions
public:	
	// Constructor
	AAreaLock( const FObjectInitializer& ObjectInitialiser );

	// Set Default Values
	void Initialise( const enum class EWinCondition& _kreUnlockCondition );

	// Open locked area
	UFUNCTION() void UnlockArea( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult );

	UFUNCTION( BlueprintImplementableEvent )
	void OpenArea();

protected:
private:

	// Member Variables
public:
protected:
private:
	enum class EWinCondition m_eUnlockCondition;

	class UBoxComponent* m_TriggerBox;
};
