// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillPushback.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillPushbackData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		TSubclassOf< class ASkillEffectPushback > m_pPushbackEffect;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fEffectDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fEffectRadius;
};

UCLASS( Blueprintable )
class USkillPushback : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillPushback();

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
	UPROPERTY()	TSubclassOf< class ASkillEffectPushback > m_pPushbackEffect;
	UPROPERTY() float m_fEffectDuration;
	UPROPERTY() float m_fEffectRadius;

};