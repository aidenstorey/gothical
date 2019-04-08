// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "InputData.generated.h"

// Prototypes
UENUM( Blueprintable )
enum EInputMethod
{
	IM_KEYBOARD		UMETA( DisplayName = "Keyboard" ),
	IM_CONTROLLER	UMETA( DisplayName = "Controller" ),
};

UENUM()
enum class EGameInputState
{
	GIS_DEFAULT,
	GIS_INVENTORY,
	GIS_CUTSCENE,
	GIS_DEBUG,
	GIS_CASTSTUN,
};

USTRUCT( BlueprintType )
struct FConfigData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Input )
		FString m_tDisplayName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Input )
		TEnumAsByte< EInputMethod > m_eInputMethod;
};

USTRUCT( BlueprintType )
struct FGamepadNameData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Input )
		FString m_tFriendlyName;

};
