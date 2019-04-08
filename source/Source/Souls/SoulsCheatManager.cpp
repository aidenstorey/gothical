// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include "GameFramework/CheatManager.h"
#include "EngineUtils.h"

// Library Includes

// Local Includes
#include "Enemy/Characters/EnemyCharacter.h"

// This Include
#include "SoulsCheatManager.h"

// Implementation

USoulsCheatManager::USoulsCheatManager( const FObjectInitializer& _krtInitialiser )
	: Super( _krtInitialiser )
{
	// Get melee enemy
	static ConstructorHelpers::FClassFinder< AEnemyCharacter > MeleeEnemy( TEXT( "/Game/Blueprints/Enemy/Characters/BP_Enemy_Melee" ) );
	this->m_EnemyClasses.Add( MeleeEnemy.Class );

	// Get ranged enemy
	static ConstructorHelpers::FClassFinder< AEnemyCharacter > RangedEnemy( TEXT( "/Game/Blueprints/Enemy/Characters/BP_Enemy_Ranged" ) );
	this->m_EnemyClasses.Add( RangedEnemy.Class );

	// Get Knight Boss
	static ConstructorHelpers::FClassFinder< AEnemyCharacter > KnightBoss( TEXT( "/Game/Blueprints/Enemy/Characters/Bosses/BP_Boss_Knight" ) );
	this->m_BossClasses.Add( KnightBoss.Class );
}

USoulsCheatManager::~USoulsCheatManager()
{
}

void USoulsCheatManager::KillAllEnemies()
{
	for( TActorIterator< AEnemyCharacter > Iterator( GetWorld() ); Iterator; ++Iterator )
	{
		Iterator->Destroy();
	}
}

void USoulsCheatManager::SpawnEnemyAbsolute( float _fX, float _fY, float _fZ, EEnemyType _EnemyType )
{
	GetWorld()->SpawnActor< AActor >( this->m_EnemyClasses[ static_cast<int>( _EnemyType ) ], FVector( _fX, _fY, _fZ ), FRotator( 0.0f, FMath::FRandRange( 0.0f, 360.0f ), 0.0f ) );
}

void USoulsCheatManager::SpawnEnemyRelative( float _fX, float _fY, float _fZ, EEnemyType _EnemyType )
{
	GetWorld()->SpawnActor< AActor >( this->m_EnemyClasses[ static_cast<int>( _EnemyType ) ], UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 )->GetActorLocation() + FVector( _fX, _fY, _fZ ), FRotator( 0.0f, FMath::FRandRange( 0.0f, 360.0f ), 0.0f ) );
}

void USoulsCheatManager::SpawnBossAbsolute( float _fX, float _fY, float _fZ, EBossType _BossType )
{
	GetWorld()->SpawnActor< AActor >( this->m_BossClasses[ static_cast<int>( _BossType ) ], FVector( _fX, _fY, _fZ ), FRotator( 0.0f, FMath::FRandRange( 0.0f, 360.0f ), 0.0f ) );
}

void USoulsCheatManager::SpawnBossRelative( float _fX, float _fY, float _fZ, EBossType _BossType )
{
	GetWorld()->SpawnActor< AActor >( this->m_BossClasses[ static_cast<int>( _BossType ) ], UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 )->GetActorLocation() + FVector( _fX, _fY, _fZ ), FRotator( 0.0f, FMath::FRandRange( 0.0f, 360.0f ), 0.0f ) );
}