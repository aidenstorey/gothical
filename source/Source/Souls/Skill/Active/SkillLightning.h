// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillLightning.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillLightningData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fDamageMultiplier;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fDamageFallOff;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fInitialDistance;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fInitialAngle;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fArcDelay;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fArcDistance;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iMaxChain;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class UParticleSystem* m_pArcParticles;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class UParticleSystem* m_pHitParticles;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fCleanupDelay;
};

UCLASS( Blueprintable )
class USkillLightning : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillLightning();

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
	UPROPERTY() float m_fDamageFallOff;
	UPROPERTY() float m_fInitialAngle;
	UPROPERTY() float m_fInitialDistance;
	UPROPERTY() float m_fArcDelay;
	UPROPERTY() float m_fArcDistance;
	UPROPERTY() int32 m_iMaxChain;
	UPROPERTY()	class UParticleSystem* m_pArcParticles;
	UPROPERTY()	class UParticleSystem* m_pHitParticles;
	UPROPERTY() float m_fCleanupDelay;

};