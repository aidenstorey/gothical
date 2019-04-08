// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include <algorithm>

// Library Includes

// Local Includes
#include "../../Attribute/Attribute.h"
#include "../../Attribute/Characteristics.h"
#include "../../HUD/WidgetBase.h"
#include "../../Inventory/Inventory.h"
#include "../../Skill/Active/SkillActive.h"
#include "../../Skill/Active/SkillBasicAttack.h"
#include "../../Skill/Interactable/SkillDrop.h"
#include "../../Skill/Interactable/SkillInteractable.h"
#include "../../Skill/SkillBase.h"
#include "../../Skill/SkillManager.h"
#include "../../SoulsCheatManager.h"
#include "../../SoulsGameInstance.h"
#include "../../SoulsGameMode.h"
#include "../Animation/AnimInstancePlayer.h"
#include "../Character/CharacterPlayer.h"

//This Include
#include "ControllerPlayer.h"

// Implementation
void AControllerPlayer::WidgetStateChange( EWidgetState _eIncomingState, EWidgetState _eCurrentState )
{	
	// TODO - Aiden:	Think about creating an adjacency matrix
	switch ( _eIncomingState )
	{
		case EWidgetState::WS_MAINMENU:
		{
			if ( _eCurrentState == EWidgetState::WS_CONTROLSMENU ||
				 _eCurrentState == EWidgetState::WS_PAUSEMENU ||
				 _eCurrentState == EWidgetState::WS_DEATHMENU ||
				 _eCurrentState == EWidgetState::WS_GRAPHICSMENU ||
				 _eCurrentState == EWidgetState::WS_WINMENU ||
				 _eCurrentState == EWidgetState::WS_CREDITS )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
				this->SetPause( true );
			}
		} break;

		case EWidgetState::WS_CONTROLSMENU:
		{
			if ( _eCurrentState == EWidgetState::WS_MAINMENU )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
			}
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			if ( _eCurrentState == EWidgetState::WS_INGAME )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
				this->SetPause( true );
			}
		} break;

		case EWidgetState::WS_DEATHMENU:
		{
			if ( _eCurrentState == EWidgetState::WS_INGAME )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
				this->SetPause( true );
			}
		} break;

		case EWidgetState::WS_INGAME:
		{

			if ( _eCurrentState == EWidgetState::WS_MAINMENU ||
				 _eCurrentState == EWidgetState::WS_DEATHMENU ||
				 _eCurrentState == EWidgetState::WS_WINMENU )
			{
				USoulsGameInstance* pInstance{ Cast< USoulsGameInstance >( this->GetGameInstance() ) };
				if ( pInstance != nullptr )
				{
					pInstance->GetSkillManager()->Initialise();
				}

				this->RestartLevel();
			}
			else if ( _eCurrentState == EWidgetState::WS_PAUSEMENU ||
					  _eCurrentState == EWidgetState::WS_CUTSCENE )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
				this->m_eGameInputState = this->m_pWidgetInstance->GetShowInventory() ? EGameInputState::GIS_INVENTORY : EGameInputState::GIS_DEFAULT;
				this->SetPause( false );
			}
		} break;
		
		case EWidgetState::WS_GRAPHICSMENU:
		{
			if( _eCurrentState == EWidgetState::WS_MAINMENU )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
			}
		} break;

		case EWidgetState::WS_WINMENU:
		{
			if ( _eCurrentState == EWidgetState::WS_INGAME )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
				this->SetPause( true );
			}
		} break;

		case EWidgetState::WS_CUTSCENE:
		{
			if ( _eCurrentState == EWidgetState::WS_INGAME )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
			}
		} break;

		case EWidgetState::WS_CREDITS:
		{
			if ( _eCurrentState == EWidgetState::WS_MAINMENU )
			{
				this->m_pWidgetInstance->SetState( _eIncomingState );
			}
		} break;

		case EWidgetState::WS_COMMAND_FEEDBACK:
		{
			FPlatformProcess::LaunchURL( *this->m_pWidgetInstance->m_tFeedbackLink, nullptr, nullptr );
		} break;

		case EWidgetState::WS_COMMAND_EXIT:
		{
			this->ConsoleCommand( "quit" );
		} break;

		default: break;
	}
}