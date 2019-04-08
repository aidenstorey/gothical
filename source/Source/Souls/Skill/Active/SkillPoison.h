// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillPoison.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillPoisonData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()	
		
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		float m_fTrailDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		float m_fTrailSpawnRate;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		TSubclassOf< class ASkillEffectPoison > m_pTrailSpawn;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		float m_fEffectLifetime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		float m_fEffectRadius;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		float m_fPoisonDuration;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills ) 
		float m_fPoisonRate;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fPoisonDamageMultiplier;
};

UCLASS( Blueprintable )
class USkillPoison : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillPoison();

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
	UPROPERTY() float m_fTrailDuration;
	UPROPERTY() float m_fTrailSpawnRate;
	UPROPERTY() TSubclassOf< class ASkillEffectPoison > m_pTrailSpawn;
	UPROPERTY() float m_fEffectLifetime;
	UPROPERTY() float m_fEffectRadius;
	UPROPERTY() float m_fPoisonDuration;
	UPROPERTY() float m_fPoisonRate;
	UPROPERTY() float m_fPoisonDamageMultiplier;
};