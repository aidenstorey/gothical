// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../EnemyControllerBase.h"

// Generated Include
#include "BossControllerBase.generated.h"

// Prototypes

UENUM( BlueprintType )
enum class EBossState : uint8
{
	BOSS_STATE_IDLE = 0			UMETA( DisplayName = "Idle" ),
	BOSS_STATE_WANDER			UMETA( DisplayName = "Wander" ),
	BOSS_STATE_SEEKING			UMETA( DisplayName = "Seeking" ),
	BOSS_STATE_FLEEING			UMETA( DisplayName = "Fleeing" ),
	
	// States for three attacks
	// So same enum can be used for all bosses
	BOSS_STATE_PRE_ATTACK_A		UMETA( DisplayName = "PreAttackA" ),
	BOSS_STATE_ATTACK_A			UMETA( DisplayName = "AttackA" ),
	BOSS_STATE_POST_ATTACK_A	UMETA( DisplayName = "PostAttackA" ),

	BOSS_STATE_PRE_ATTACK_B		UMETA( DisplayName = "PreAttackB" ),
	BOSS_STATE_ATTACK_B			UMETA( DisplayName = "AttackB" ),
	BOSS_STATE_POST_ATTACK_B	UMETA( DisplayName = "PostAttackB" ),

	BOSS_STATE_PRE_ATTACK_C		UMETA( DisplayName = "PreAttackC" ),
	BOSS_STATE_ATTACK_C			UMETA( DisplayName = "AttackC" ),
	BOSS_STATE_POST_ATTACK_C	UMETA( DisplayName = "PostAttackC" ),

	// Hit state
	BOSS_STATE_PRE_HIT			UMETA( DisplayName = "PreHit" ),
	BOSS_STATE_HIT				UMETA( DisplayName = "Hit" ),
	BOSS_STATE_POST_HIT			UMETA( DisplayName = "PostHit" )
};

UCLASS( abstract )
class SOULS_API ABossControllerBase : public AEnemyControllerBase
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructor
	ABossControllerBase( const FObjectInitializer& _krtInitialiser );

	// UE4 Overrides
	virtual void Tick( float DeltaSeconds ) override;

	// Boss specialised functions
	// Accessor Functions
	EBossState GetCurrentBossState() const;
	void SetCurrentBossState( const EBossState& _kreNewState );
	
	float GetBossCooldown( const int& _kreAttackIndex );
	void SetBossCooldown( const int& _kreAttackIndex, const float& _krfNewCooldown );

	// Blueprint Events
	UFUNCTION( BlueprintImplementableEvent, Category = "Boss" )
	void OnStateChange( EBossState _eNewState );

	UFUNCTION( BlueprintCallable, Category = "Boss" )
	void EnterHitState( float _fAnimDuration ) override;

	// Overridden from EnemyControllerBase
	virtual void ProcessState() override;
	virtual FVector GetGoToLocation() override;

protected:
private:
	
	// Member Variables
public:
protected:
private:
	UPROPERTY() EBossState		m_eCurrentBossState;
	UPROPERTY() float			m_fCooldowns[ 3 ];
};
