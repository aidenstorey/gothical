// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillFreeze.generated.h"

// Prototypes
USTRUCT( BlueprintType ) 
struct FSkillFreezeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		TSubclassOf< class ASkillEffectFreeze > m_pEffect;		

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fStunDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fEffectDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fEffectSpeed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fEffectAngle;

};

UCLASS( Blueprintable )
class USkillFreeze : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillFreeze();

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
	UPROPERTY() TSubclassOf< class ASkillEffectFreeze > m_pEffect;
	UPROPERTY() float m_fStunDuration;
	UPROPERTY() float m_fEffectDuration;
	UPROPERTY() float m_fEffectSpeed;
	UPROPERTY() float m_fEffectAngle;

};