// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes

//Generated Include
#include "WidgetState.generated.h"

// Types
UENUM( BlueprintType )
enum class EWidgetState : uint8
{
	WS_INGAME			UMETA( DisplayName = "InGame" ),
	WS_MAINMENU			UMETA( DisplayName = "MainMenu" ),
	WS_CONTROLSMENU		UMETA( DisplayName = "ControlsMenu" ),
	WS_PAUSEMENU		UMETA( DisplayName = "PauseMenu" ),
	WS_DEATHMENU		UMETA( DisplayName = "DeathMenu" ),
	WS_GRAPHICSMENU		UMETA( DisplayName = "GraphicsMenu" ),
	WS_WINMENU			UMETA( DisplayName = "WinMenu" ),
	WS_CUTSCENE			UMETA( DisplayName = "CutScene" ),
	WS_CREDITS			UMETA( DisplayName = "Credits" ),

	WS_COMMAND_FEEDBACK	UMETA( DisplayName = "FeedbackCommand" ),
	WS_COMMAND_EXIT UMETA( Displayname = "ExitCommand" ),

	MAX_WS,
};