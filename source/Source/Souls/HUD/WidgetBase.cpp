// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes
#include <algorithm>

// Local Includes
#include "../Attribute/Characteristics.h"
#include "../Inventory/Inventory.h"
#include "../Skill/SkillBase.h"
#include "../Skill/Active/SkillActive.h"
#include "../SoulsGameInstance.h"
#include "../Player/Controller/ControllerPlayer.h"
#include "WidgetHealthOverlay.h"
#include "WidgetHotbarBadge.h"
#include "WidgetInventoryIcon.h"
#include "WidgetInteractableOverlay.h"
#include "WidgetMenuButton.h"

//This Include
#include "WidgetBase.h"

// Implementation
EWidgetState UWidgetBase::GetState() const
{
	return ( this->m_eState );
}

void UWidgetBase::SetState( const EWidgetState& _kreState )
{
	if ( _kreState != EWidgetState::WS_INGAME )
	{
		this->m_bShowInventory = false;
	}

	this->m_eState = _kreState;
	this->MenuSelectedReset();
}

FText UWidgetBase::GetSelectedName()
{
	FText tReturn;

	if ( this->m_pInventory != nullptr )
	{
		USkillBase* pSkill{ this->m_pInventory->GetSlotContents( this->m_iInventoryIndex ) };
		if ( pSkill != nullptr )
		{
			tReturn = FText::FromString( pSkill->GetDisplayName().ToString().ToUpper() );
		}
	}

	return ( tReturn );
}

FText UWidgetBase::GetSelectedDescription()
{
	FText tReturn;

	if ( this->m_pInventory != nullptr )
	{
		USkillBase* pSkill{ this->m_pInventory->GetSlotContents( this->m_iInventoryIndex ) };
		if ( pSkill != nullptr )
		{
			tReturn = FText::FromString( pSkill->GetToolTip().ToString().ToUpper() );
		}
	}

	return ( tReturn );
}

bool UWidgetBase::GetShowDebugText()
{
	bool bReturn{ false };

	if ( this->GetOwningPlayer() != nullptr )
	{
		USoulsGameInstance* pInstance{ Cast< USoulsGameInstance >( this->GetOwningPlayer()->GetGameInstance() ) };
		if ( pInstance != nullptr )
		{
			bReturn = pInstance->GetShowDebugScreen();
		}
	}

	return ( bReturn );
}

FText UWidgetBase::GetDebugScreenText()
{
	FText tReturn;

	if ( this->m_pOwner != nullptr )
	{
		tReturn = this->m_pOwner->GetDebugScreenText();
	}

	return ( tReturn );
}

void UWidgetBase::SetSelected( int32 _iRow, int32 _iCol )
{
	this->m_iInventoryRowSelected = _iRow;
	this->m_iInventoryColSelected = _iCol;

	this->m_arrInventoryIcons[ this->m_iInventoryIndex ]->SetSelected( false );
	this->m_iInventoryIndex = ( ( this->m_iInventoryRowSelected + 1 ) * this->m_iInventoryColCount + this->m_iInventoryColSelected ) % ( this->m_iInventoryRowCount * this->m_iInventoryColCount );
	this->m_arrInventoryIcons[ this->m_iInventoryIndex ]->SetSelected( true );
}

void UWidgetBase::SetMenuSelected( int32 _iOption, TArray< class UWidgetMenuButton* >& _rarrMenuButtons )
{
	for ( int iButton{ 0 }; iButton < _rarrMenuButtons.Num(); iButton++ )
	{
		_rarrMenuButtons[ iButton ]->SetButtonState( iButton == _iOption ? EMenuButtonState::BS_HOVER : EMenuButtonState::BS_NORMAL );
	}
}

ESlateVisibility UWidgetBase::GetDebugScreenVisibility()
{
	return ( this->GetShowDebugText() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden );
}

bool UWidgetBase::GetShowInventory() const
{
	return ( this->m_bShowInventory );
}

void UWidgetBase::SetShowInventory( const bool& _krbShow )
{
	this->m_bShowInventory = _krbShow;
	this->InventorySelectedReset();

	for ( auto pBadge : this->m_arrHotbarBadges )
	{
		pBadge->SetShowInventory( this->m_bShowInventory );
	}
}

void UWidgetBase::ToggleShowInventory()
{
	if ( this->m_eState == EWidgetState::WS_INGAME )
	{
		this->m_bShowInventory = !this->m_bShowInventory;
		this->InventorySelectedReset();

		for ( auto pBadge : this->m_arrHotbarBadges )
		{
			pBadge->SetShowInventory( this->m_bShowInventory );
		}
	}
}

int UWidgetBase::GetInventoryIndex() const
{
	return ( this->m_iInventoryIndex );
}

TEnumAsByte< EInputMethod > UWidgetBase::GetInputMethod() const
{
	return ( this->m_eInputMethod );
}

void UWidgetBase::SetInputMethod( TEnumAsByte< EInputMethod > _eInputMethod )
{
	this->m_eInputMethod = _eInputMethod;
	switch ( this->m_eInputMethod )
	{
		case EInputMethod::IM_CONTROLLER:
		{
			UWidgetInteractableOverlay::SetButtonName( "A" ); // TODO:	Get this from the actual controls
		} break;

		case EInputMethod::IM_KEYBOARD:
		{
			UWidgetInteractableOverlay::SetButtonName( "E" ); // TODO:	Get this from the actual controls
		} break;
	}

	for ( auto pBadge : this->m_arrHotbarBadges )
	{
		pBadge->SetInputMethod( this->m_eInputMethod );
	}
}

void UWidgetBase::ToggleInputMethod()
{
	switch ( this->m_eInputMethod )
	{
		case EInputMethod::IM_CONTROLLER:
		{
			this->m_eInputMethod = EInputMethod::IM_KEYBOARD;
			UWidgetInteractableOverlay::SetButtonName( "A" ); // TODO:	Get this from the actual controls
		} break;

		case EInputMethod::IM_KEYBOARD:
		{
			this->m_eInputMethod = EInputMethod::IM_CONTROLLER;
			UWidgetInteractableOverlay::SetButtonName( "E" ); // TODO:	Get this from the actual controls
		} break;
	}

	if ( this->m_pOwner != nullptr )
	{
		this->m_pOwner->SetInputMethod( this->m_eInputMethod );
	}
}

FStateChange& UWidgetBase::GetOnStateChangeDelegate()
{
	return ( this->m_tOnStateChange );
}

bool UWidgetBase::IsInMenu()
{
	return ( this->m_eState == EWidgetState::WS_MAINMENU ||
			 this->m_eState == EWidgetState::WS_CONTROLSMENU ||
			 this->m_eState == EWidgetState::WS_PAUSEMENU ||
			 this->m_eState == EWidgetState::WS_DEATHMENU ||
			 this->m_eState == EWidgetState::WS_GRAPHICSMENU ||
			 this->m_eState == EWidgetState::WS_WINMENU ||
			 this->m_eState == EWidgetState::WS_CREDITS );
}

bool UWidgetBase::IsMouseOverMenu()
{
	return ( this->m_bMouseOver );
}
float UWidgetBase::GetHealthMin() const
{
	return ( this->m_fHealthMin );
}

float UWidgetBase::GetHealthMax() const
{
	return ( this->m_fHealthMax );
}

void UWidgetBase::SetHealth( float _fPercent )
{
	this->m_fHealthDesired = _fPercent;
	this->m_fHealthMin = _fPercent;
	this->m_fHealthMax = _fPercent;
}

void UWidgetBase::SetHealthDesired( float _fPercent )
{
	this->m_fHealthDesired = _fPercent;
	this->m_bMinDepleting = this->m_fHealthDesired < this->m_fHealthMin;
	this->m_bMaxDepleting = this->m_fHealthDesired < this->m_fHealthMax;
}

void UWidgetBase::Initialise( class AControllerPlayer* _pOwner )
{
	this->m_pOwner = _pOwner;
	this->m_pInventory = this->m_pOwner->GetInventory();

	for ( auto pIcon : this->m_arrInventoryIcons )
	{
		pIcon->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrMainMenuButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrPauseMenuButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrDeathMenuButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrWinMenuButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrControlsMenuButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrGraphicsMenuButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( auto pButton : this->m_arrCreditsButtons )
	{
		pButton->SetOwningWidget( this );
	}

	for ( int iBadge{ 0 }; iBadge < this->m_arrHotbarBadges.Num() && iBadge < this->m_arrInventoryIcons.Num(); iBadge++ )
	{
		this->m_arrInventoryIcons[ iBadge ]->SetBadge( this->m_arrHotbarBadges[ iBadge ] );
	}

	this->m_fInputCooldownDuration = 0.0f;
}

void UWidgetBase::ChangeState( EWidgetState _eState )
{
	this->m_tOnStateChange.Broadcast( _eState, this->m_eState );
}

ESlateVisibility UWidgetBase::DetermineVisibility( EWidgetState _eCheck, ESlateVisibility _eTrue, ESlateVisibility _eFalse )
{
	return ( _eCheck == this->m_eState ? _eTrue : _eFalse );
}

ESlateVisibility UWidgetBase::DetermineInventoryVisibility()
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	if ( this->m_eState == EWidgetState::WS_INGAME &&
		 this->m_bShowInventory )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}

ESlateVisibility UWidgetBase::DetermineInputConfigVisibility( EInputMethod _eInputMethod )
{
	ESlateVisibility eReturn{ ESlateVisibility::Hidden };

	if ( this->m_eInputMethod == _eInputMethod && 
		 this->m_eState == EWidgetState::WS_CONTROLSMENU )
	{
		eReturn = ESlateVisibility::Visible;
	}

	return ( eReturn );
}

ESlateVisibility UWidgetBase::DetermineControllerButtonVisibility()
{
	return ( this->m_eInputMethod != EInputMethod::IM_CONTROLLER ? ESlateVisibility::Visible : ESlateVisibility::Hidden );
}

ESlateVisibility UWidgetBase::DetermineKeyboardButtonVisibility()
{
	return ( this->m_eInputMethod != EInputMethod::IM_KEYBOARD ? ESlateVisibility::Visible : ESlateVisibility::Hidden );
}

void UWidgetBase::InputPause()
{
	switch ( this->m_eState )
	{
		case EWidgetState::WS_INGAME:
		{
			this->ChangeState( EWidgetState::WS_PAUSEMENU );
		} break;

		case EWidgetState::WS_WINMENU:
		case EWidgetState::WS_PAUSEMENU:
		case EWidgetState::WS_DEATHMENU:
		case EWidgetState::WS_MAINMENU:
		{
			this->ChangeState( EWidgetState::WS_INGAME );
		} break;

		default: break;
	}
}

void UWidgetBase::InputBack()
{
	switch ( this->m_eState )
	{
		case EWidgetState::WS_INGAME:
		{
			if ( this->m_bShowInventory )
			{
				this->SetShowInventory( false );
				this->m_pOwner->SetInputState( EGameInputState::GIS_DEFAULT );
			}
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			this->ChangeState( EWidgetState::WS_INGAME );
		} break;

		case EWidgetState::WS_WINMENU:
		case EWidgetState::WS_DEATHMENU:
		case EWidgetState::WS_CONTROLSMENU:
		case EWidgetState::WS_GRAPHICSMENU:
		case EWidgetState::WS_CREDITS:
		{
			this->ChangeState( EWidgetState::WS_MAINMENU );
		} break;
	}
}

void UWidgetBase::InputEscape()
{
	switch ( this->m_eState )
	{
		case EWidgetState::WS_INGAME:
		{
			if ( this->m_bShowInventory )
			{
				this->SetShowInventory( false );
				this->m_pOwner->SetInputState( EGameInputState::GIS_DEFAULT );
			}
			else
			{
				this->ChangeState( EWidgetState::WS_PAUSEMENU );
			}
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			this->ChangeState( EWidgetState::WS_INGAME );
		} break;
		
		case EWidgetState::WS_WINMENU:
		case EWidgetState::WS_DEATHMENU:
		case EWidgetState::WS_CONTROLSMENU:
		case EWidgetState::WS_GRAPHICSMENU:
		case EWidgetState::WS_CREDITS:
		{
			this->ChangeState( EWidgetState::WS_MAINMENU );
		} break;
	}
}

void UWidgetBase::InputNavigate( EMoveDirection _eDirection )
{
	switch ( this->m_eState )
	{
		case EWidgetState::WS_INGAME:
		{
			if ( this->m_bShowInventory )
			{
				this->InventorySelectedMove( _eDirection );
			}
			else
			{
				// TODO - Aiden:	Potential other options
			}
		} break;

		case EWidgetState::WS_MAINMENU:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrMainMenuButtons );
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrPauseMenuButtons );
		} break;

		case EWidgetState::WS_DEATHMENU:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrDeathMenuButtons );
		} break;

		case EWidgetState::WS_WINMENU:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrWinMenuButtons );
		} break;

		case EWidgetState::WS_CONTROLSMENU:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrControlsMenuButtons );
		} break;

		case EWidgetState::WS_GRAPHICSMENU:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrGraphicsMenuButtons );
		} break;

		case EWidgetState::WS_CREDITS:
		{
			this->MenuSelectedMove( _eDirection, this->m_arrCreditsButtons );
		} break;

		default: break;
	}
}

void UWidgetBase::InventorySelectedMove( EMoveDirection _eDirection )
{
	if ( !this->m_bOnInputCooldown )
	{
		this->m_bOnInputCooldown = true;
		this->m_fInputTimeElapsed = 0.0f;

		switch ( _eDirection )
		{
			case EMoveDirection::MD_UP:
			{
				this->m_iInventoryRowSelected = ( this->m_iInventoryRowSelected + this->m_iInventoryRowCount - 1 ) % this->m_iInventoryRowCount;
			} break;

			case EMoveDirection::MD_DOWN:
			{
				this->m_iInventoryRowSelected = ( this->m_iInventoryRowSelected + 1 ) % this->m_iInventoryRowCount;
			} break;

			case EMoveDirection::MD_LEFT:
			{
				this->m_iInventoryColSelected = ( this->m_iInventoryColSelected + this->m_iInventoryColCount - 1 ) % this->m_iInventoryColCount;
			} break;

			case EMoveDirection::MD_RIGHT:
			{
				this->m_iInventoryColSelected = ( this->m_iInventoryColSelected + 1 ) % this->m_iInventoryColCount;
			} break;

			default: break;
		}

		this->SetSelected( this->m_iInventoryRowSelected, this->m_iInventoryColSelected );
	}
}


void UWidgetBase::InventorySelectedReset()
{
	this->SetSelected( 0, 0 );
}

void UWidgetBase::MenuSelectedMouse( int32 _iSelected )
{
	this->m_iMenuIndex = _iSelected;

	switch ( this->m_eState )
	{
		case EWidgetState::WS_MAINMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrMainMenuButtons );
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrPauseMenuButtons );
		} break;

		case EWidgetState::WS_DEATHMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrDeathMenuButtons );
		} break;

		case EWidgetState::WS_WINMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrWinMenuButtons );
		} break;

		default: break;
	}

	this->m_bMouseOver = true;
}

void UWidgetBase::MenuDeselectMouse()
{
	this->m_bMouseOver = false;
}

void UWidgetBase::MenuActivateSelected()
{
	switch ( this->m_eState )
	{
		case EWidgetState::WS_MAINMENU:
		{
			this->ChangeState( this->m_arrMainMenuButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			this->ChangeState( this->m_arrPauseMenuButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;

		case EWidgetState::WS_DEATHMENU:
		{
			this->ChangeState( this->m_arrDeathMenuButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;

		case EWidgetState::WS_WINMENU:
		{
			this->ChangeState( this->m_arrWinMenuButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;
		case EWidgetState::WS_CONTROLSMENU:
		{
			this->ChangeState( this->m_arrControlsMenuButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;

		case EWidgetState::WS_GRAPHICSMENU:
		{
			this->ChangeState( this->m_arrGraphicsMenuButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;

		case EWidgetState::WS_CREDITS:
		{
			this->ChangeState( this->m_arrCreditsButtons[ this->m_iMenuIndex ]->m_eChangeToState );
		} break;

		default: break;
	}
}

void UWidgetBase::MenuSelectedMove( EMoveDirection _eDirection, TArray< class UWidgetMenuButton* >& _rarrMenuButtons )
{
	if ( !this->m_bOnInputCooldown )
	{
		this->m_bOnInputCooldown = true;
		this->m_fInputTimeElapsed = 0.0f;

		switch ( _eDirection )
		{
			case EMoveDirection::MD_UP:
			{
				this->m_iMenuIndex = ( this->m_iMenuIndex + _rarrMenuButtons.Num() - 1 ) % _rarrMenuButtons.Num();
			} break;

			case EMoveDirection::MD_DOWN:
			{
				this->m_iMenuIndex = ( this->m_iMenuIndex + 1 ) % _rarrMenuButtons.Num();
			} break;

			default: break;
		}

		this->SetMenuSelected( this->m_iMenuIndex, _rarrMenuButtons );
	}
}

void UWidgetBase::MenuSelectedReset()
{
	this->m_iMenuIndex = 0;

	switch ( this->m_eState )
	{
		case EWidgetState::WS_MAINMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrMainMenuButtons );
		} break;

		case EWidgetState::WS_PAUSEMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrPauseMenuButtons );
		} break;

		case EWidgetState::WS_DEATHMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrDeathMenuButtons );
		} break;

		case EWidgetState::WS_WINMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrWinMenuButtons );
		} break;
		case EWidgetState::WS_CONTROLSMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrControlsMenuButtons );
		} break;

		case EWidgetState::WS_GRAPHICSMENU:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrGraphicsMenuButtons );
		} break;

		case EWidgetState::WS_CREDITS:
		{
			this->SetMenuSelected( this->m_iMenuIndex, this->m_arrCreditsButtons );
		} break;

		default: break;
	}
}

void UWidgetBase::Process( float _fDeltaTime )
{
	if ( this->m_bOnInputCooldown )
	{
		this->m_fInputTimeElapsed += _fDeltaTime;
		if ( this->m_fInputTimeElapsed > this->m_fInputCooldownDuration )
		{
			this->m_bOnInputCooldown = false;
			this->m_fInputTimeElapsed = 0.0f;
		}
	}

	if ( this->m_fHealthMin != this->m_fHealthDesired )
	{
		if ( this->m_bMinDepleting )
		{
			this->m_fHealthMin = std::max( this->m_fHealthDesired, this->m_fHealthMin - _fDeltaTime );
		}
		else
		{
			this->m_fHealthMin = std::min( this->m_fHealthDesired, this->m_fHealthMin + _fDeltaTime );
		}
	}
}