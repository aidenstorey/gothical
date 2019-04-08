// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Base/ProjectileBase.h"

//Generated Include
#include "ProjectilePlayer.generated.h"

// Prototypes
class AEnemyCharacter;

UCLASS( Blueprintable )
class AProjectilePlayer : public AProjectileBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	AProjectilePlayer( const FObjectInitializer& _rtInitialiser );

	// Accessors
	void SetHomingDistance( float _fDistance );

	// UE4 Overrides
	virtual void ReceiveTick( float _fDeltaTime ) override;

	// Delegate
	UFUNCTION()
	void OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult );

	void StopHoming();

protected:
private:

	// Member Variables
public:
protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Projectile Components" )
	class USphereComponent* m_pHomingTrigger;

	float m_fHomingDistance;

	AEnemyCharacter* m_pHomingTarget = nullptr;

private:

};