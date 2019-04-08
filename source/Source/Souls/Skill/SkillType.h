// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "SkillType.generated.h"

// Types
UENUM( BlueprintType )
enum class ESkillClassType : uint8
{
	ST_PASSIVE,
	ST_ACTIVE_EARTH_QUAKE,
	ST_ACTIVE_FIREBALL,
	ST_ACTIVE_POISON,
	ST_ACTIVE_LIGHTNING,
	ST_ACTIVE_PUSHBACK,
	ST_ACTIVE_LIFESTEAL,
	ST_ACTIVE_FREEZE,
	ST_BUFF_HEALER,
	ST_ACTIVE_BASIC_ATTACK,

	MAX_ST,
};