// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillInteractable.h"

//Generated Include
#include "SkillDrop.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ASkillDrop : public ASkillInteractable
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	ASkillDrop( const FObjectInitializer& _krtInitialiser );

protected:
	// Other
	virtual bool ReceiveInteract( class UInventory* _pInventory ) override;

private:

	// Member Variables
public:
protected:
private:

};