//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS( Blueprintable )
class SOULS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Spawns member of m_tSpawnables
	void SpawnActor();

protected:
private:

public:
	UPROPERTY( EditAnywhere )
	UClass*	m_tSpawnable;

	// UPROPERTY()
	// TArray< UClass >	m_tSpawnables;
	UPROPERTY( EditAnywhere ) float m_fCooldownTime;

	UPROPERTY( EditAnywhere )
	int32 m_iSurroundingMax = 10;

protected:
private:
	UPROPERTY() float	m_fCooldown;
};
