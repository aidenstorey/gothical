// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillActive.h"

//Generated Include
#include "SkillFireball.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillDataFireball : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSkillDataFireball() 
	{
		// ...
	}

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		TSubclassOf< class AProjectileFireball > m_pProjectileClass;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fSpeed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fRange;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fInitialDamageMultiplyer;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fExplosionRadius;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		float m_fExplosionDamageMultiplyer;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iExplosionMaxTargets;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		bool m_bExplosionDamageFalloff;
};

UCLASS( Blueprintable )
class USkillFireball : public USkillActive
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillFireball();

protected:
	// Other 
	virtual void ReceiveInitialise() override;

	virtual bool ReceiveActivate() override;
	virtual bool ReceiveDeactivate() override;
	virtual bool ReceiveTrigger() override;

private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() TSubclassOf< class AProjectileFireball > m_pProjectileClass;
	UPROPERTY() float m_fSpeed;
	UPROPERTY() float m_fRange;
	UPROPERTY() float m_fInitialDamageMultiplyer;
	UPROPERTY() float m_fExplosionRadius;
	UPROPERTY() float m_fExplosionDamageMultiplyer;
	UPROPERTY() int32 m_iExplosionMaxTargets;
	UPROPERTY() bool m_bExplosionDamageFalloff;

};