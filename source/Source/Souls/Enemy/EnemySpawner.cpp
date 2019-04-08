//// Fill out your copyright notice in the Description page of Project Settings.
//
#include "Souls.h"
#include "EnemySpawner.h"

#include "Characters/EnemyCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner() :
	m_fCooldown( 0.0f )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//// Called every frame
void AEnemySpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if( this->m_fCooldown > 0.0f )
	{
		this->m_fCooldown -= DeltaTime;
	}
	else
	{
		this->SpawnActor();
		this->m_fCooldown = this->m_fCooldownTime; // TODO - Ethan : Magic Numbers
	}

}

void AEnemySpawner::SpawnActor()
{
	// TODO - Ethan : Make this function actually work

	TArray< FOverlapResult > OverlapList;
	this->GetWorld()->OverlapMultiByObjectType( OverlapList,
												this->GetActorLocation(),
												FQuat::Identity,
												FCollisionObjectQueryParams( ECC_TO_BITFIELD( ECC_GameTraceChannel3 ) ), // Enemy Collision Channel
												FCollisionShape::MakeSphere( 500.0f ) );
	
	int iCurrentSpawnables = 0;
	
	for( int iIndex{ 0 }; iIndex < OverlapList.Num(); iIndex++ )
	{
		if( Cast< AEnemyCharacter >( OverlapList[ iIndex ].GetActor() ) != nullptr )
		{
			iCurrentSpawnables++;
		}
	}
	
	if( iCurrentSpawnables < m_iSurroundingMax )
	{
		this->GetWorld()->SpawnActor< AActor >( m_tSpawnable,//&m_tSpawnables[ FMath::RandRange( 0, m_tSpawnables.Num() ) ],
												this->GetActorLocation() + ( FVector( FMath::FRand(), 0.0f, 0.0f ).RotateAngleAxis( FMath::FRandRange( 0.0f, 360.0f ), FVector( 0.0f, 0.0f, 1.0f ) ) * 500.0f ),
												FRotator( 0.0f, FMath::FRandRange( 0.0f, 360.0f ), 0.0f ) );
	}

}