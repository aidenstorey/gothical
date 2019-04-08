// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/PlayerController.h"
#include "Templates/SharedPointer.h"

// Library Includes

// Local Includes
#include "../../HUD/InputData.h"
#include "../../HUD/WidgetState.h"
#include "../../Skill/SkillType.h"
#include "../Animation/AnimStatePlayer.h"

// Generated Include
#include "ControllerPlayer.generated.h"

// Prototypes
UCLASS( Blueprintable, Config=Game )
class AControllerPlayer : public APlayerController
{
	GENERATED_BODY()
		
	// Member Types
public:
protected:
private:

	// Member Functions
public:
	// Constructors
	AControllerPlayer( const FObjectInitializer& _krtInitialiser );

	// Accessors
	UFUNCTION( BlueprintCallable, Category = "Souls" ) class UCharacteristics* GetCharacteristics();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) class UInventory* GetInventory();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) class UWidgetBase* GetCurrentWidget();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) class UAnimInstancePlayer* GetAnimInstancePlayer();

	UFUNCTION( BlueprintCallable, Category = "Souls" ) void SetInputMethod( EInputMethod _eMethod );
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void SetInputState( EGameInputState _eInputState );

	UFUNCTION( BlueprintCallable, Category = "Souls" ) void SetAnimState( EAnimStatePlayer _eState );
	UFUNCTION( BlueprintCallable, Category = "Souls" ) bool IsAnimState( EAnimStatePlayer _eState );

	UFUNCTION( BlueprintPure, Category = "Souls" ) FText GetDebugScreenText();

	UFUNCTION() bool IsLightningActive() const;

	// UE4 Overrides
	virtual void BeginPlay() override;
	virtual void PlayerTick( float _fDeltaTime ) override;

	// Other
	UFUNCTION() void SetupInputController();
	UFUNCTION() void SetupInputKeyboard();

	UFUNCTION() void Teleport( FVector _vPosition, bool _bSnapTerrain = true );
	UFUNCTION() void TeleportRelative( FVector _vPosition, bool _bSnapTerrain = true );

	UFUNCTION() void BeginPoisonTrail( float _fTrailDuration,
									   float _fTrailSpawnRate,
									   TSubclassOf< class ASkillEffectPoison > _pTrailSpawn,
									   float _fEffectLifetime,
									   float _fEffectRadius,
									   float _fPoisonDuration, 
									   float _fPoisonRate,
									   int32 _iPoisonAmount );
	UFUNCTION() void EndPoisonTrail();

	UFUNCTION() bool BeginLightning( class AEnemyCharacter* pInitialTarget,
									 float _fDamageMultiplier,
									 float _fDamageFallOff,
									 float _fArcDelay,
									 float _fArcDistance,
									 int32 _iMaxChain,
									 class UParticleSystem* _pArcParticles,
									 class UParticleSystem* _pHitParticles,
									 float _fCleanupDelay );

	UFUNCTION() void EndLightning();
	UFUNCTION() void LightningTarget( class AEnemyCharacter* pTarget, float _fDamageAmount );

	UFUNCTION() void HotkeyPressed( int32 _iKey );

	UFUNCTION( BlueprintCallable, Category = "Souls" ) void BeginCutScene();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void EndCutScene();

	UFUNCTION( BlueprintCallable, Category = "Souls" ) void BasicAttackEnter();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void BasicAttackLeave();

	UFUNCTION( BlueprintCallable, Category = "Souls" ) bool ActivateSkill( class USkillActive* _pSkill );
	UFUNCTION( BlueprintCallable, Category = "Souls" ) bool DeactivateSkill();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) bool TriggerSkill();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) bool InteruptSkill();
	
	// Bound
	UFUNCTION() void InputGeneralMoveForward( float _fAxisValue );
	UFUNCTION() void InputGeneralMoveRight( float _fAxisValue );
	UFUNCTION() void InputGeneralLookForward( float _fAxisValue );
	UFUNCTION() void InputGeneralLookRight( float _fAxisValue );

	UFUNCTION() void InputGeneralInventory();
	UFUNCTION() void InputGeneralNavigateUp();
	UFUNCTION() void InputGeneralNavigateDown();
	UFUNCTION() void InputGeneralNavigateLeft();
	UFUNCTION() void InputGeneralNavigateRight();

	UFUNCTION() void InputGeneralInteract();
	UFUNCTION() void InputMouseActivateSelected();

	UFUNCTION() void InputGeneralToggleDebug();

	UFUNCTION() void InputGeneralHotkey1();
	UFUNCTION() void InputGeneralHotkey2();
	UFUNCTION() void InputGeneralHotkey3();
	UFUNCTION() void InputGeneralHotkey4();
	UFUNCTION() void InputGeneralDrop();

	UFUNCTION() void WidgetStateChange( EWidgetState _eIncomingState, EWidgetState _eCurrentState );
	UFUNCTION() void ReceiveWin();
	UFUNCTION() void ReceiveDeath();
	UFUNCTION() void DropSkillEffect( class USkillBase* _pSkill );
	UFUNCTION() void MovementAttributeModified( int32 _iAmount );
	UFUNCTION() void AttackSpeedAttributeModified( int32 _iAmount );
	UFUNCTION() void HealthAlterFailed();
	
	// Command 
	UFUNCTION( Exec ) void PlayerSetHealth( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerSetHealthMax( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerSetDamage( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerSetAttackSpeed( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerSetMovementSpeed( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerFullHeal();
	UFUNCTION( Exec ) void PlayerHeal( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerRegen( float _fTime, float _fRate );
	UFUNCTION( Exec ) void PlayerInvincibilityActivate();
	UFUNCTION( Exec ) void PlayerInvincibilityDeactivate();
	UFUNCTION( Exec ) void PlayerKill();
	UFUNCTION( Exec ) void PlayerTakeDamage( int32 _iAmount );
	UFUNCTION( Exec ) void PlayerInventoryAdd( int32 _iType );
	UFUNCTION( Exec ) void PlayerInventoryAddSlot( int32 _iSlotNumber, int32 _iType );
	UFUNCTION( Exec ) void PlayerInventoryAddCustomPassive( int32 _iHealth, int32 _iDamage, int32 _iAttackSpeed, int32 _iMomementSpeed );
	UFUNCTION( Exec ) void PlayerInventoryFill(int32 _iType);
	UFUNCTION( Exec ) void PlayerInventoryFillRandom();
	UFUNCTION( Exec ) void PlayerInventoryDrop( int32 _iSlotNumber );
	UFUNCTION( Exec ) void PlayerInventoryDropAll();
	UFUNCTION( Exec ) void PlayerInventoryRemove( int32 _iSlotNumber );
	UFUNCTION( Exec ) void PlayerInventoryRemoveAll();
	UFUNCTION( Exec ) void PlayerInventorySwap( int32 _iSlotNumberA, int32 _iSlotNumberB );
	UFUNCTION( Exec ) void PlayerInputSetMethod( int32 _iMethod );
	UFUNCTION( Exec ) void PlayerWinConditionToggleDisabled();
	UFUNCTION( Exec ) void PlayerWinConditionAdd( int32 _iCondition );
	UFUNCTION( Exec ) void PlayerWinConditionRemove( int32 _iCondition );
	UFUNCTION( Exec ) void PlayerMovementTeleportSnap( float _fX, float _fY );
	UFUNCTION( Exec ) void PlayerMovementTeleport( float _fX, float _fY, float _fZ );
	UFUNCTION( Exec ) void PlayerMovementTeleportRelativeSnap( float _fX, float _fY );
	UFUNCTION( Exec ) void PlayerMovementTeleportRelative( float _fX, float _fY, float _fZ );

protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls" ) USoundCue* m_pLightningSoundCue;

protected:
private:
	UPROPERTY( Config ) TEnumAsByte< EInputMethod > m_eInputMethod;
	UPROPERTY( Config ) bool m_bWinScreenDisabled;
	
	EGameInputState m_eGameInputState;
	UPROPERTY() UInputComponent* m_pInputController;
	UPROPERTY() UInputComponent* m_pInputKeyboard;

	UPROPERTY() FVector m_vFacingVector;
	
	UPROPERTY() class UCharacteristics* m_pCharacteristics;
	UPROPERTY() class UInventory* m_pInventory;

	UPROPERTY() TSubclassOf< class UWidgetBase > m_pWidgetClass;
	UPROPERTY() class UWidgetBase* m_pWidgetInstance;

	UPROPERTY() class UAnimInstancePlayer* m_pAnimInstance;

	UPROPERTY() bool m_bAttacking;
	UPROPERTY() class USkillBasicAttack* m_pBasicAttack;
	UPROPERTY() class USkillActive* m_pActiveSkill;

	UPROPERTY() FVector m_Destination;

	UPROPERTY() bool m_bPreWinActive;
	UPROPERTY() float m_fPreWinDuration;
	UPROPERTY() float m_fPreWinRemaining;
	UPROPERTY() float m_fPreWinTimeDilation;

	UPROPERTY() bool m_bPoisonTrailActive;
	UPROPERTY() float m_fPoisonTrailDuration;
	UPROPERTY() float m_fPoisonTrailTimeElapsed;
	UPROPERTY() float m_fPoisonTrailSpawnRate;
	UPROPERTY() TSubclassOf< class ASkillEffectPoison > m_pPoisonTrailSpawn;
	UPROPERTY() float m_fPoisonEffectLifetime;
	UPROPERTY() float m_fPoisonEffectRadius;
	UPROPERTY() float m_fPoisonDuration;
	UPROPERTY() float m_fPoisonRate;
	UPROPERTY() int32 m_iPoisonAmount;

	UPROPERTY() bool m_bLightningActive;
	UPROPERTY() class AEnemyCharacter* m_pLightningTargetCurr;
	UPROPERTY() class AEnemyCharacter* m_pLightningTargetPrev;
	UPROPERTY() int32 m_iLightningDamageAmount;
	UPROPERTY() float m_fLightningDamageFallOff;
	UPROPERTY() float m_fLightningArcDelay;
	UPROPERTY() float m_fLightningTimeElapsed;
	UPROPERTY() float m_fLightningArcDistance;
	UPROPERTY() int32 m_iLightningMaxChain;
	UPROPERTY()	class UParticleSystem* m_pLightningArcParticles;
	UPROPERTY()	class UParticleSystem* m_pLightningHitParticles;
	UPROPERTY() TArray< class AEnemyCharacter* > m_arrLightningTargets;

	UPROPERTY() bool m_bLightningCleanup;
	UPROPERTY() float m_fLightningCleanupDelay;
	UPROPERTY() float m_fLightningCleanupTimeElapsed;
	TQueue< class UParticleSystemComponent* > m_queueLightningParticles;
};