// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes

// Generated Include
#include "Characteristics.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FCharacteristicsNotifyDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FTakeDamage, int32, _iAmount );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FHealthChange, float, _fDesiredPercent );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FPoisonNotify );

// Prototypes
USTRUCT( BlueprintType )
struct FCharacteristicData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FCharacteristicData() : 
		m_iHealth( 0 ),
		m_iDamage( 0 ),
		m_iAttackSpeed( 0 ),
		m_iMovementSpeed( 0 )
	{}

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Characteristics )
		int32 m_iHealth;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Characteristics )
		int32 m_iDamage;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Characteristics )
		int32 m_iAttackSpeed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Characteristics )
		int32 m_iMovementSpeed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Characteristics )
		int32 m_iRange;
};

UCLASS()
class UCharacteristics : public UObject
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	UCharacteristics( const FObjectInitializer& _krtInitialiser );

	// Accessors
	int32 GetHealth() const;
	class UAttribute* GetHealthAttribute();
	
	UFUNCTION( BlueprintCallable, Category = "Souls" ) int32 GetDamage() const;
	class  UAttribute* GetDamageAttribute();
	
	int32 GetAttackSpeed() const;
	class UAttribute* GetAttackSpeedAttribute();
	
	int32 GetMovementSpeed() const;
	class UAttribute* GetMovementSpeedAttribute();

	int32 GetRange() const;
	class UAttribute* GetRangeAttribute();

	bool GetInvincible() const;
	void SetInvincible( const bool& _krbInvincible );

	UFUNCTION( BlueprintCallable, Category = "Souls" ) float GetHealthPercent() const;
	
	FTakeDamage& GetOnTakeDamageDelegate();
	FCharacteristicsNotifyDelegate& GetOnDeathDelegate();
	FCharacteristicsNotifyDelegate& GetOnHealDelegate();
	FHealthChange& GetOnHealthChangeDelegate();
	FHealthChange& GetOnHealthSetDelegate();
	FPoisonNotify& GetOnPoisonBeginDelegate();
	FPoisonNotify& GetOnPoisonEndDelegate();

	// Other
	void Initialise( const FName& _krtRowName );
	void Process( float _fDeltaTime );

	void Heal();
	void Heal( const int32& _kriAmount );
	void Heal( const float& _krfPercent );

	void Regen( float _fTime, float _fRate, int32 _iRegenAmount = 1 );
	void Poison( float _fTime, float _fRate, int32 _iDamageAmount = 1 );

	void ActivateInvincibility(); 
	void DeactivateInvincibility();

	void Kill();
	void TakeDamage( const int32& _kriAmount, AActor* _pLastHit = nullptr );

	bool AttachModifier( class UAttributeModifier* _pModifier );
	bool DetachModifier( class UAttributeModifier* _pModifier );

protected:
	UFUNCTION() void HealthChanged();

private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() class UAttribute* m_pHealth;
	UPROPERTY() class UAttribute* m_pDamage;
	UPROPERTY() class UAttribute* m_pAttackSpeed;
	UPROPERTY() class UAttribute* m_pMovementSpeed;
	UPROPERTY() class UAttribute* m_pRange;

	UPROPERTY() TMap< FString, class UAttribute* > m_mapAttributes;

	UPROPERTY() UDataTable* m_DataTable;

	UPROPERTY() bool m_bInvincible;

	UPROPERTY() bool m_bInPoison;
	UPROPERTY() float m_fPoisonTime;
	UPROPERTY() float m_fPoisonRate;
	UPROPERTY() float m_fPoisonElapsed;
	UPROPERTY() int32 m_iPoisonAmount;
	
	FTakeDamage m_tOnTakeDamage;
	FCharacteristicsNotifyDelegate m_tOnDeath;
	FCharacteristicsNotifyDelegate m_tOnHeal;
	FHealthChange m_tOnHealthChange;
	FHealthChange m_tOnHealthSet;
	FPoisonNotify m_tOnPoisonBegin;
	FPoisonNotify m_tOnPoisonEnd;
};