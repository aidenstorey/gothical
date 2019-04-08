// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "AnimStatePlayer.generated.h"

// Prototypes
UENUM( BlueprintType )
enum class EAnimStatePlayer : uint8
{
	ASP_IDLE_WALK_RUN,
	ASP_BASIC_ATTACK,
	ASP_PUSHBACK,
	ASP_FREEZE,
	ASP_LIGHTNING,
	ASP_HEALER,
	ASP_EARTHQUAKE,
	ASP_FIREBALL,
	ASP_POISON,
	ASP_LIFESTEAL,

	MAX_ASP,
};