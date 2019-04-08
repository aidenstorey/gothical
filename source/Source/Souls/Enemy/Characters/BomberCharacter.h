// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "Enemy/Characters/EnemyCharacter.h"

// Generate Include
#include "BomberCharacter.generated.h"

// Prototypes

UCLASS()
class SOULS_API ABomberCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructor
	ABomberCharacter( const FObjectInitializer& ObjectInitializer );

	virtual float TakeDamage( float _fDamageAmount, struct FDamageEvent const& _krtDamageEvent, class AController* _pEventInstigator, AActor* _pDamageCauser ) override;

	// Enemy behaviour
	void Explode();

	// Delegate
	UFUNCTION()
	void OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult );

protected:
private:
	
	// Member Variables
public:
protected:
private:
	UPROPERTY( VisibleAnywhere, Category = "Enemy Components" )
	class USphereComponent* TriggerSphere;
};
