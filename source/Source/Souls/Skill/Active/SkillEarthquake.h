// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillEarthquake.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillEarthquakeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		TSubclassOf< class UCameraShake > m_pCameraShake;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iDamageMultiplier;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iRadius;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iMaxTargets;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		UMaterialInterface* m_pDecal;
};

UCLASS( Blueprintable )
class USkillEarthquake : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillEarthquake();

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
	TSubclassOf< class UCameraShake > m_pCameraShake;
	int32 m_iDamageMultiplier;
	int32 m_iRadius;
	int32 m_iMaxTargets;
	UMaterialInterface* m_pDecal;

};