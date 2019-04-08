// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes
#include <algorithm>

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Enemy/Characters/EnemyCharacter.h"
#include "../Controller/ControllerPlayer.h"

//This Include
#include "ProjectileFireball.h"

// Implementation
void AProjectileFireball::InitialiseExplosion( float _fRadius, float _fDamageMultiplyer, int32 _iMaxTargets, bool _bFalloff )
{
	this->m_fExplosionRadius = _fRadius;
	this->m_fExplosionDamageMultiplyer = _fDamageMultiplyer;
	this->m_iExplosionMaxTargets = _iMaxTargets;
	this->m_bExplosionDamageFalloff = _bFalloff;
}

void AProjectileFireball::Destroyed()
{
	auto pController = Cast< AControllerPlayer >( this->m_pOwner );
	if ( pController != nullptr )
	{
		// Perform a sphere collision to detect enemies
		TArray< FOverlapResult > arrOverlapList;
		this->GetWorld()->OverlapMultiByObjectType( arrOverlapList, this->GetActorLocation(), FQuat::Identity,
													FCollisionObjectQueryParams::InitType::AllDynamicObjects,
													FCollisionShape::MakeSphere( static_cast< float >( this->m_fExplosionRadius ) ),
													FCollisionQueryParams{ TEXT( "FireballExplosion" ), false, this } );

		this->SpawnDestroyedParticle( this->GetActorLocation() );

		// Initialise required damage variables
		FDamageEvent tDamageEvent;
		tDamageEvent.DamageTypeClass = UDamageType::StaticClass();
		int32 iDamageAmount{ pController->GetCharacteristics()->GetDamage() * static_cast< int32 >( this->m_fExplosionDamageMultiplyer ) };

		FVector vPosition{ this->GetActorLocation() };
		float fFalloffRatio{ 1.0f / this->m_fExplosionRadius };

		// Iterate over possible hit enemies
		for ( int32 iIndex{ 0 }, iTargetCount{ 0 }; iIndex < arrOverlapList.Num() && iTargetCount < this->m_iExplosionMaxTargets; iIndex++ )
		{
			// Check if target is an enemy
				// True - apply the modified damage amount 
			auto pEnemy = Cast< AEnemyCharacter >( arrOverlapList[ iIndex ].GetActor() );
			if ( pEnemy != nullptr )
			{
				// Check if damage falloff is active
					// True - apply damage calculation
				float fDamageFalloff{ 1.0f };
				if ( this->m_bExplosionDamageFalloff )
				{
					fDamageFalloff = fFalloffRatio * FVector{ vPosition - pEnemy->GetActorLocation() }.Size();
				}

				pEnemy->TakeDamage( static_cast< float >( iDamageAmount ) * fDamageFalloff, tDamageEvent, pController, pController->GetCharacter() );
				iTargetCount++;
			}
		}
	}

}