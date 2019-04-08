// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "SkillData.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSkillData() :
		m_pIcon( nullptr ),
		m_pSkillDrop( nullptr )
	{}

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fCooldownTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		bool m_bCastStun;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		FText m_tToolTip;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		UTexture2D* m_pIcon;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		UClass* m_pSkillDrop;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class UParticleSystem* m_pActivateParticles;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class UParticleSystem* m_pDeactivateParticles;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class UParticleSystem* m_pTriggerParticles;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class USoundCue* m_pSoundCueActivate;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class USoundCue* m_pSoundCueRecurring;

};