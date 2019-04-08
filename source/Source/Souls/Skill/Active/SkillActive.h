// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../SkillBase.h"

//Generated Include
#include "SkillActive.generated.h"

// Prototypes
UCLASS( Blueprintable )
class USkillActive : public USkillBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	float GetCooldown() const;
	void SetCooldown( const float& _krfCooldown, const bool& _krbUpdateTimeRemaining = false );

	float GetTimeRemaining() const;
	void SetTimeRemaining( const float& _krfTimeRemaining, const bool& _krbClampToCooldown = false );

	virtual float GetCooldownPercentage() const override;

	virtual bool OnCooldown() const override;

	// Other
	virtual bool Process( const float& _krfDeltaTime ) override;

protected:
	// Other
	virtual bool ReceiveInterupt() override;

private:

	// Member Variables
public:
protected:
	UPROPERTY() bool m_bTriggered;
	UPROPERTY() float m_fTimeRemaining;

private:

};