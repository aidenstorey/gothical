// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../SkillBase.h"

//Generated Include
#include "SkillPassive.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FPassiveSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FPassiveSkillData() : 
		m_iModifierMin( 0 ),
		m_iModifierMax( 0 ),
		m_iModifierOffset( 0 )
	{}

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Passives )
		int32 m_iModifierMin;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Passives )
		int32 m_iModifierMax;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Passives )
		int32 m_iModifierOffset;
};

UCLASS( Blueprintable )
class USkillPassive : public USkillBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillPassive( const FObjectInitializer& _krtInitialiser );

	// Accessors
	virtual int32 PassiveIncreaseCount() const override;

	bool HasHealthModifier() const;
	bool HasDamageModifier() const;
	bool HasAttackSpeedModifier() const;
	bool HasMovementSpeedModifier() const;

	// Other
	void CustomiseModifiers( int32 _iHealth, int32 _iDamage, int32 _iAttackSpeed, int32 _iMovementSpeed );

protected:
	// Accessors
	bool HasModifier( FString _tName ) const;

	// Other
	virtual void ReceiveInitialise() override;
	virtual bool ReceiveActivate() override;
	virtual bool ReceiveDeactivate() override;
	virtual bool ReceiveInterupt() override;

	void AddModifier( FString _tName, int32 _iAmount );

private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() TArray< class UAttributeModifier* > m_arrModifiers;
	UPROPERTY() UDataTable* m_DataTable;
	UPROPERTY() bool m_bCustomCreated;
};