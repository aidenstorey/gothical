// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Base/CharacterBase.h"

// Generated Include
#include "EnemyCharacter.generated.h"

// Prototypes

UCLASS( Blueprintable )
class SOULS_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

	// Member Functions
public:
	AEnemyCharacter( const FObjectInitializer& _krtInitialiser );
	// UE4 Overrides
	virtual float TakeDamage( float _fDamageAmount, struct FDamageEvent const& _krtDamageEvent, class AController* _pEventInstigator, AActor* _pDamageCauser ) override;

	UFUNCTION( BlueprintCallable, Category = "Tick" )
	void SetAllTicks( bool _NewTick );
	void ToggleAllTicks();

	virtual void BeginPlay() override;

	UFUNCTION( BlueprintImplementableEvent, Category = "Damage" )
	void SpawnIndicator( const float& _krfDamageAmount );
	
	UFUNCTION( BlueprintImplementableEvent, Category = "Colour" )
	void StartHit();
	UFUNCTION( BlueprintImplementableEvent, Category = "Colour" )
	void StartPoison();
	UFUNCTION( BlueprintImplementableEvent, Category = "Colour" )
	void EndPoison();
	UFUNCTION( BlueprintImplementableEvent, Category = "Colour" )
	void StartFreeze();
	UFUNCTION( BlueprintImplementableEvent, Category = "Colour" )
	void EndFreeze();

	void AddSeeker( class AProjectilePlayer* _pNewSeeker );
	void StopSeekers();

	// Other
	UFUNCTION() void BeginPoisonParticles();
	UFUNCTION() void EndPoisonParticles();

	void FreezeCharacter();
	void UnfreezeCharacter();

	UFUNCTION( BlueprintCallable, Category = "Animation" )
	void FreezeAnimation();
	UFUNCTION( BlueprintCallable, Category = "Animation" )
	void UnfreezeAnimation();

protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( Displayname = "Poison Particles" ) ) class UParticleSystem* m_pPoisonParticles;

protected:
private:
	TArray< class AProjectilePlayer* > m_pCurrentSeekers;
	UParticleSystemComponent* m_pActiveParticles;
};
