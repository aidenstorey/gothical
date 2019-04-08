// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillBasicAttack.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillDataBasic : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSkillDataBasic()
	{}

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		TSubclassOf< class AProjectilePlayer > m_ProjectileClass;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fSpeed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fHalfOffset;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fHomingDistance;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iMovementReduction;
};

UCLASS()
class USkillBasicAttack : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	USkillBasicAttack();

protected:
	// Other 
	virtual void ReceiveInitialise() override;

	virtual bool ReceiveActivate() override;
	virtual bool ReceiveDeactivate() override;
	virtual bool ReceiveInterupt() override;
	virtual bool ReceiveTrigger() override;

private:

	// Member Variables
public:
protected:
	UPROPERTY() TSubclassOf< class AProjectilePlayer > m_ProjectileClass;
	UPROPERTY() bool m_bFirstAttack;
	UPROPERTY() float m_fSpeed;
	UPROPERTY() float m_fHalfOffset;
	UPROPERTY() float m_fHomingDistance;
	UPROPERTY() int32 m_iMovementReduction;
	UPROPERTY() class UAttributeModifier* m_pModifier;

private:
};