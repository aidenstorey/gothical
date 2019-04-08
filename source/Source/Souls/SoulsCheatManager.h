// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

// Generated Include
#include "SoulsCheatManager.generated.h"

// Prototypes
class UCheatManager;

UENUM()
enum class EEnemyType : uint8
{
	ENEMY_TYPE_MELEE = 0		UMETA( DisplayName = "Melee" ),
	ENEMY_TYPE_RANGED			UMETA( DisplayName = "Ranged" ),
	ENEMY_TYPE_MAX				UMETA( DisplayName = "MaxValue" )
};

UENUM()
enum class EBossType : uint8
{
	BOSS_TYPE_KNIGHT = 0		UMETA( DisplayName = "Knight" )
};

UCLASS()
class USoulsCheatManager : public UCheatManager
{
	GENERATED_BODY()

	// Member Functions
public:
	USoulsCheatManager( const FObjectInitializer& _krtInitialiser );
	~USoulsCheatManager();

	UFUNCTION( Exec )
	void KillAllEnemies();

	UFUNCTION( Exec )
	void SpawnEnemyAbsolute( float _fX, float _fY, float _fZ, EEnemyType _EnemyType );

	UFUNCTION( Exec )
	void SpawnEnemyRelative( float _fX, float _fY, float _fZ, EEnemyType _EnemyType );

	UFUNCTION( Exec )
	void SpawnBossAbsolute( float _fX, float _fY, float _fZ, EBossType _BossType );
	
	UFUNCTION( Exec )
	void SpawnBossRelative( float _fX, float _fY, float _fZ, EBossType _BossType );

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() TArray< TSubclassOf< class AEnemyCharacter > > m_EnemyClasses;
	UPROPERTY() TArray< TSubclassOf< class AEnemyCharacter > > m_BossClasses;
};
