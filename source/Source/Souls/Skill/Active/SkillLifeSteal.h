// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillLifeSteal.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillLifeStealData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fDamageMultiplier;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fRadius;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iMaxTargets;
};

UCLASS( Blueprintable )
class USkillLifeSteal : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillLifeSteal();

	// Bound
	UFUNCTION() void ParticleComplete();

protected:
	// Other
	virtual void ReceiveInitialise() override;
	virtual bool ReceiveActivate() override;
	virtual bool ReceiveTrigger() override;

private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() float m_fDamageMultiplier;
	UPROPERTY() float m_fRadius;
	UPROPERTY() int32 m_iMaxTargets;
	UPROPERTY() int32 m_iHealAmount;
};