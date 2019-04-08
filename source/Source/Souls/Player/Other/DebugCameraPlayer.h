// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "DebugCameraPlayer.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ADebugCameraPlayer : public ADebugCameraController
{
	GENERATED_BODY()

	// Member Functions
public:
	// UE4 Overrides
	virtual void BeginPlay() override;
	
	// Bound
	UFUNCTION() void TeleportPlayer();

protected:
private:

	// Member Variables
public:
protected:
private:

};