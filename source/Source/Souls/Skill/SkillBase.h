// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes

//Generated Include
#include "SkillBase.generated.h"

// Prototypes
UCLASS( Blueprintable )
class USkillBase : public UObject
{
	GENERATED_BODY()

	// Member Types
public:
	enum class ESkillType : uint8
	{
		ST_ACTIVE,
		ST_CONSUME,
		ST_PASSIVE,
	};

protected:
private:

	// Member Functions
public:
	// Constructors
	USkillBase();

	// Accessors
	ESkillType GetType() const;
	
	class UTexture2D* GetTexture() const;
	
	FText GetToolTip() const;
	FText GetDisplayName() const;
	class UClass* GetSkillDrop() const;

	class AControllerPlayer* GetOwningController() const;
	void SetOwningController( class AControllerPlayer* _pController );
	bool SetOwningInventorySlot( class UInventorySlot* _pInventorySlot );

	void SetActivated( bool _bState );
	bool IsActivated() const;
	virtual bool OnCooldown() const;
	virtual bool HasCastStun() const;

	virtual float GetCooldownPercentage() const;

	virtual int32 PassiveIncreaseCount() const;

	// Other 
	virtual void Initialise();

	void LoadSkillInfo();

	virtual bool Activate();
	virtual bool Deactivate();
	virtual bool Trigger();
	virtual bool Interupt();
	virtual bool Process( const float& _krfDeltaTime );
	
protected:
	// Other 
	virtual void ReceiveInitialise();

	virtual bool ReceiveActivate();
	virtual bool ReceiveDeactivate();
	virtual bool ReceiveTrigger();
	virtual bool ReceiveInterupt();
	virtual bool ReceiveProcess( const float& _krfDeltatime );
	
private:

	// Member Variables
public:
protected:
	ESkillType m_eType;
	UPROPERTY() FText m_tDisplayName;
	UPROPERTY() bool m_bCastStun;

	UPROPERTY() bool m_bActivated;
	
	UPROPERTY() float m_fCooldown;
	UPROPERTY() class UTexture2D* m_pTexture;
	UPROPERTY() FText m_tToolTip;
	UPROPERTY() class UClass* m_pSkillDrop;
	
	UPROPERTY() class AControllerPlayer* m_pOwningController;
	UPROPERTY() class UInventorySlot* m_pOwningInventorySlot;

	UPROPERTY()	class UParticleSystem* m_pActivateParticles;
	UPROPERTY()	class UParticleSystem* m_pDeactivateParticles;
	UPROPERTY()	class UParticleSystem* m_pTriggerParticles;

	UPROPERTY() UDataTable* m_pSkillDataTable;
	UPROPERTY() UDataTable* m_pSkillStatsDataTable;
	UPROPERTY() class USoundCue* m_pSoundCueActivate;
	UPROPERTY() class USoundCue* m_pSoundCueRecurring;

private:

};