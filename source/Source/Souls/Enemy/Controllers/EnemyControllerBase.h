// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "AIController.h"

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"

// Generated Include
#include "EnemyControllerBase.generated.h"

// Prototypes

UENUM( BlueprintType )
enum class EEnemyState : uint8
{
	ENEMY_STATE_IDLE = 0		UMETA( DisplayName = "Idle" ),
	ENEMY_STATE_WANDER			UMETA( DisplayName = "Wander" ),
	ENEMY_STATE_SEEKING			UMETA( DisplayName = "Seeking" ),
	ENEMY_STATE_FLEEING			UMETA( DisplayName = "Fleeing" ),
	ENEMY_STATE_PRE_ATTACK		UMETA( DisplayName = "PreAttack" ),
	ENEMY_STATE_ATTACK			UMETA( DisplayName = "Attack" ),
	ENEMY_STATE_POST_ATTACK		UMETA( DisplayName = "PostAttack" ),
	ENEMY_STATE_PRE_EXPLODE		UMETA( DisplayName = "PreExplode" ),
	ENEMY_STATE_EXPLODE			UMETA( DisplayName = "Explode" ),
	ENEMY_STATE_POST_EXPLODE	UMETA( DisplayName = "PostExplode" ),
	ENEMY_STATE_PRE_HIT			UMETA( DisplayName = "PreHit" ),
	ENEMY_STATE_HIT				UMETA( DisplayName = "Hit" ),
	ENEMY_STATE_POST_HIT		UMETA( DisplayName = "PostHit" )
};

UCLASS( abstract )
class AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructor
	AEnemyControllerBase( const FObjectInitializer& _krtInitialiser );

	// UE4 Overrides
	virtual void Tick( float DeltaSeconds ) override;
	virtual void BeginPlay() override;

	// Accessor Functions
	EEnemyState GetCurrentState() const;
	void SetCurrentState( const EEnemyState& _kreNewState );

	float GetCurrentCooldown() const;
	void SetCurrentCooldown( const float& _krfNewCooldown );

	float GetCurrentHitCooldown() const;
	void SetCurrentHitCooldown( const float& _krfNewHitCooldown );

	void SetName( const FName& _krtName );

	UFUNCTION( BlueprintCallable, Category = "CharacterEnemy" )
	class UCharacteristics* GetCharacteristics();

	// Blueprint Events
	UFUNCTION( BlueprintImplementableEvent, Category = "CharacterEnemy" )
	void OnStateChange( EEnemyState _eNewState );

	// Enemy Behaviour
	virtual bool DetectPlayer();
	virtual void ProcessState();
	virtual FVector GetGoToLocation();
	virtual void Attack();

	void AlertOtherEnemies();

	// Blueprint Functions
	UFUNCTION( BlueprintCallable, Category = "CharacterEnemy" )
	virtual void EnterHitState( float _fAnimDuration );

	// Bound 
	UFUNCTION() void Death();
	UFUNCTION() void ReceivedDamage( int32 _iAmount );

	UFUNCTION( BlueprintCallable, Category = "EnemyTick" )
	void ToggleTick();

	UFUNCTION( BlueprintCallable, Category = "EnemyTick" )
	void SetTick( bool _NewTick );

	UFUNCTION() void BeginPoisonParticles();
	UFUNCTION() void EndPoisonParticles();

	UFUNCTION() void FreezeEnemy( float _fFreezeLength );
	UFUNCTION() void UnfreezeEnemy();

protected:
	// Other
	virtual void ReceiveDeath();

private:

	// Member Variables
public:
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) FName			m_Name;
	UPROPERTY() float			m_fHitCooldown; // Prevents enemy getting locked in hitstun

protected:
private:
	UPROPERTY() EEnemyState		m_eCurrentState;
	UPROPERTY() float			m_fCooldown;

	UPROPERTY() bool			m_bFrozen;			// Not the best way of doing it
	UPROPERTY() float			m_fFreezeLength;	// But I didn't plan for it earlier

	UPROPERTY() UCharacteristics* m_pCharacteristics;
};