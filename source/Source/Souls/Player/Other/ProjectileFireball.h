// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Base/ProjectileBase.h"

//Generated Include
#include "ProjectileFireball.generated.h"

// Prototypes
UCLASS( Blueprintable )
class AProjectileFireball : public AProjectileBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Other 
	void InitialiseExplosion( float _fRadius, float _fDamageMultiplyer, int32 _iMaxTargets, bool _bFalloff );

	UFUNCTION( BlueprintImplementableEvent, Category = "Souls" ) void SpawnDestroyedParticle( FVector _vLocation );

	// UE4 override
	virtual void Destroyed() override;


protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() float m_fExplosionRadius;
	UPROPERTY() float m_fExplosionDamageMultiplyer;
	UPROPERTY() int32 m_iExplosionMaxTargets;
	UPROPERTY() bool m_bExplosionDamageFalloff;

};