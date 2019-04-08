// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../SkillBase.h"

//Generated Include
#include "SkillConsume.generated.h"

// Prototypes
UCLASS( Blueprintable )
class USkillConsume : public USkillBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Other
	virtual void Initialise() override;
	virtual bool Activate() override;

protected:
private:

	// Member Variables
public:
protected:
private:

};