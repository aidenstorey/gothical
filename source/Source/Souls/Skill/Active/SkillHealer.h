// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillHealer.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillHealerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fRate;
};

UCLASS( Blueprintable )
class USkillHealer : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillHealer();

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
	UPROPERTY() float m_fDuration;
	UPROPERTY() float m_fRate;

};