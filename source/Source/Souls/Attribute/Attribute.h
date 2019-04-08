// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes

//Generated Include
#include "Attribute.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FModifiedDelegate, int32, _iAmount );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FAlterFailDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FRegenNotify );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FAttributeChanged );

// Prototypes
UCLASS()
class UAttribute : public UObject
{
	GENERATED_BODY()

	// Member Types
public:
	UENUM()
	enum class EAttributeType : uint8
	{
		ET_STATIC,
		ET_DYNAMIC,

		MAX_ET,
	};

protected:
private:

	// Member Functions
public:
	// Constructors
	UAttribute();

	// Accessors
	FString GetName() const;

	EAttributeType GetType() const;
	void SetType( const EAttributeType& _kreType );

	int32 GetMin() const;
	void SetMin( const int32& _kriMin );

	int32 GetMax() const;
	void SetMax( const int32& _kriMax );

	int32 GetValue() const;
	void SetValue( const int32& _kriValue );

	float GetRegenRate() const;
	void SetRegenRate( const float& _krfRate );
	
	FModifiedDelegate& GetOnModifiedDelegate();
	FAlterFailDelegate& GetOnAlterFailDelegate();
	FRegenNotify& GetOnRegenBeginDelegate();
	FRegenNotify& GetOnRegenEndDelegate();
	FAttributeChanged& GetOnAttributeChangedDelegate();
	
	// Other
	void Initialise( const FString& _krstrName, const EAttributeType& _kreType, const int32& _kriMin, const int32& _kriMax, const int32& _kriValue );
	void Process( float _fDeltaTime );

	void Add( int32 _iAmount );
	void Remove( int32 _iAmount );

	void Regen( float _fTime, float _fRate, int32 _iRegenAmount );

	bool AttachModifier( class UAttributeModifier* _pModifier );
	bool DetachModifier( class UAttributeModifier* _pModifier );

protected:
	// Other
	void ClampValue();
	UFUNCTION() void CalculateModifiedMaxStatic();
	UFUNCTION() bool CalculateModifiedMaxDynamic();

private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() FString m_strName;
	UPROPERTY() EAttributeType m_eType;

	UPROPERTY() int32 m_iValueMin;
	UPROPERTY() int32 m_iValueMax;
	UPROPERTY() int32 m_iValueModifiedMax;
	UPROPERTY() int32 m_iValue;

	UPROPERTY() bool m_bInRegen;
	UPROPERTY() float m_fRegenTime;
	UPROPERTY() float m_fRegenRate;
	UPROPERTY() float m_fRegenElapsed;
	UPROPERTY() int32 m_iRegenAmount;

	UPROPERTY() TArray< class UAttributeModifier* > m_arrModifiers;

	FModifiedDelegate m_tOnModified;
	FAlterFailDelegate m_tOnAlterFail;

	FRegenNotify m_tOnRegenBegin;
	FRegenNotify m_tOnRegenEnd;

	FAttributeChanged m_tOnAttributeChanged;
};