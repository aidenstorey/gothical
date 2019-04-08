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
void AControllerPlayer::SetInputMethod( EInputMethod _eMethod )
{
	this->m_eInputMethod = _eMethod;

	switch ( this->m_eInputMethod )
	{
		case EInputMethod::IM_CONTROLLER:
		{
			this->SetupInputController();
		} break;

		case EInputMethod::IM_KEYBOARD:
		{
			this->SetupInputKeyboard();
		} break;

		default: break;
	}

	if ( this->m_pWidgetInstance != nullptr )
	{
		this->m_pWidgetInstance->SetInputMethod( this->m_eInputMethod );
	}

	this->SaveConfig();
}

void AControllerPlayer::SetInputState( EGameInputState _eInputState )
{
	this->m_eGameInputState = _eInputState;
}

void AControllerPlayer::SetupInputController()
{
	if ( this->m_pInputController == nullptr )
	{
		this->m_pInputController = NewObject< UInputComponent >( this, "InputController" );
		this->m_pInputController->BindAxis( "C_Move_Forward", this, &AControllerPlayer::InputGeneralMoveForward );
		this->m_pInputController->BindAxis( "C_Move_Right", this, &AControllerPlayer::InputGeneralMoveRight );
		this->m_pInputController->BindAxis( "C_Look_Forward", this, &AControllerPlayer::InputGeneralLookForward );
		this->m_pInputController->BindAxis( "C_Look_Right", this, &AControllerPlayer::InputGeneralLookRight );

		this->m_pInputController->BindAction( "C_Pause", EInputEvent::IE_Pressed, this->m_pWidgetInstance, &UWidgetBase::InputPause ).bExecuteWhenPaused = true;
		this->m_pInputController->BindAction( "C_Back", EInputEvent::IE_Pressed, this->m_pWidgetInstance, &UWidgetBase::InputBack ).bExecuteWhenPaused = true;
		this->m_pInputController->BindAction( "C_Navigate_Up", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateUp ).bExecuteWhenPaused = true;
		this->m_pInputController->BindAction( "C_Navigate_Down", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateDown ).bExecuteWhenPaused = true;
		this->m_pInputController->BindAction( "C_Navigate_Left", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateLeft ).bExecuteWhenPaused = true;
		this->m_pInputController->BindAction( "C_Navigate_Right", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateRight ).bExecuteWhenPaused = true;

		this->m_pInputController->BindAction( "C_Inventory", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralInventory );
		this->m_pInputController->BindAction( "C_Hotkey_1", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey1 );
		this->m_pInputController->BindAction( "C_Hotkey_2", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey2 );
		this->m_pInputController->BindAction( "C_Hotkey_3", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey3 );
		this->m_pInputController->BindAction( "C_Hotkey_4", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey4 );
		this->m_pInputController->BindAction( "C_Drop", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralDrop );

		this->m_pInputController->BindAction( "C_Interact", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralInteract );
		this->m_pInputController->BindAction( "C_Select", EInputEvent::IE_Pressed, this->m_pWidgetInstance, &UWidgetBase::MenuActivateSelected ).bExecuteWhenPaused = true;
		this->m_pInputController->BindAction( "M_Click", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputMouseActivateSelected ).bExecuteWhenPaused = true;

		// Debug binding
		this->m_pInputController->BindAction( "K_Toggle_Debug", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralToggleDebug ).bExecuteWhenPaused = true;
	}
	
	this->InputComponent = this->m_pInputController;
}

void AControllerPlayer::SetupInputKeyboard()
{
	if ( this->m_pInputKeyboard == nullptr )
	{
		this->m_pInputKeyboard = NewObject< UInputComponent >( this, "InputKeyboard" );
		this->m_pInputKeyboard->BindAxis( "K_Move_Forward", this, &AControllerPlayer::InputGeneralMoveForward );
		this->m_pInputKeyboard->BindAxis( "K_Move_Back", this, &AControllerPlayer::InputGeneralMoveForward );
		this->m_pInputKeyboard->BindAxis( "K_Move_Right", this, &AControllerPlayer::InputGeneralMoveRight );
		this->m_pInputKeyboard->BindAxis( "K_Move_Left", this, &AControllerPlayer::InputGeneralMoveRight );
		this->m_pInputKeyboard->BindAxis( "K_Look_Forward", this, &AControllerPlayer::InputGeneralLookForward );
		this->m_pInputKeyboard->BindAxis( "K_Look_Back", this, &AControllerPlayer::InputGeneralLookForward );
		this->m_pInputKeyboard->BindAxis( "K_Look_Right", this, &AControllerPlayer::InputGeneralLookRight );
		this->m_pInputKeyboard->BindAxis( "K_Look_Left", this, &AControllerPlayer::InputGeneralLookRight );

		this->m_pInputKeyboard->BindAction( "K_Escape", EInputEvent::IE_Pressed, this->m_pWidgetInstance, &UWidgetBase::InputEscape ).bExecuteWhenPaused = true;
		this->m_pInputKeyboard->BindAction( "K_Navigate_Up", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateUp ).bExecuteWhenPaused = true;
		this->m_pInputKeyboard->BindAction( "K_Navigate_Down", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateDown ).bExecuteWhenPaused = true;
		this->m_pInputKeyboard->BindAction( "K_Navigate_Left", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateLeft ).bExecuteWhenPaused = true;
		this->m_pInputKeyboard->BindAction( "K_Navigate_Right", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralNavigateRight ).bExecuteWhenPaused = true;

		this->m_pInputKeyboard->BindAction( "K_Inventory", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralInventory );
		this->m_pInputKeyboard->BindAction( "K_Hotkey_1", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey1 );
		this->m_pInputKeyboard->BindAction( "K_Hotkey_2", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey2 );
		this->m_pInputKeyboard->BindAction( "K_Hotkey_3", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey3 );
		this->m_pInputKeyboard->BindAction( "K_Hotkey_4", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralHotkey4 );
		this->m_pInputKeyboard->BindAction( "K_Drop", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralDrop );

		this->m_pInputKeyboard->BindAction( "K_Interact", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralInteract );
		this->m_pInputKeyboard->BindAction( "K_Enter", EInputEvent::IE_Pressed, this->m_pWidgetInstance, &UWidgetBase::MenuActivateSelected ).bExecuteWhenPaused = true;
		this->m_pInputKeyboard->BindAction( "M_Click", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputMouseActivateSelected ).bExecuteWhenPaused = true;

		// Debug binding
		this->m_pInputKeyboard->BindAction( "K_Toggle_Debug", EInputEvent::IE_Pressed, this, &AControllerPlayer::InputGeneralToggleDebug ).bExecuteWhenPaused = true;
	}

	this->InputComponent = this->m_pInputKeyboard;
}

void AControllerPlayer::HotkeyPressed( int32 _iKey )
{
	switch ( this->m_eGameInputState )
	{
		case EGameInputState::GIS_DEFAULT:
		{
			auto pSkill = this->m_pInventory->GetSlotContents( _iKey );
			if ( pSkill != nullptr && pSkill->GetType() == USkillBase::ESkillType::ST_ACTIVE )
			{
				pSkill->Activate();
			}
		} break;

		case EGameInputState::GIS_INVENTORY:
		{
			if ( this->m_pInventory->GetSlotContents( this->m_pWidgetInstance->GetInventoryIndex() ) != nullptr )
			{
				this->m_pInventory->SwapSkill( _iKey, this->m_pWidgetInstance->GetInventoryIndex() );
			}
		} break;
		 
		default: break;
	}
}

void AControllerPlayer::InputGeneralMoveForward( float _fAxisValue )
{
	// Check axis value isn't 0
	if ( _fAxisValue != 0.0f && !this->m_pWidgetInstance->GetShowInventory() && this->m_eGameInputState == EGameInputState::GIS_DEFAULT )
	{
		// Determine forward rotation
		FRotator tRotation{ this->GetControlRotation() };

		// Limit pitch when walking or falling
		APawn* pControlledPawn{ this->GetPawn() };
		if ( pControlledPawn->GetMovementComponent()->IsMovingOnGround() ||
				pControlledPawn->GetMovementComponent()->IsFalling() )
		{
			tRotation.Pitch = 0.0f;
		}

		// Add movement in forward direction
		const FVector tDirection{ FRotationMatrix( tRotation ).GetScaledAxis( EAxis::X ) };
		
		// m_Destination.X = ( this->GetPawn()->GetActorLocation().X + ( 100.0f * _fAxisValue ) );

		FHitResult Hit( ForceInit );
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams( FName( TEXT( "RV_Trace" ) ), true, this );
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->LineTraceSingleByChannel( Hit,
											  GetCharacter()->GetActorLocation() + FVector( 100.0f * _fAxisValue, 0.0f, 100.0f ),
											  GetCharacter()->GetActorLocation() + FVector( 100.0f * _fAxisValue, 0.0f, -500.0f ),
											  ECC_Visibility,
											  RV_TraceParams );

		// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Magenta, Hit.ImpactPoint.ToCompactString() );
		// 
		// DrawDebugLine( GetWorld(), GetCharacter()->GetActorLocation() + FVector( 100.0f * _fAxisValue, 0.0f, 100.0f ),
		// 						   GetCharacter()->GetActorLocation() + FVector( 100.0f * _fAxisValue, 0.0f, -300.0f ),
		// 						   FColor::Red, true, 10.0f );

		float fAngle = FVector::DotProduct( Hit.ImpactNormal, GetCharacter()->GetActorUpVector() );
		fAngle /= ( Hit.ImpactNormal.Size() * GetCharacter()->GetActorUpVector().Size() );
		fAngle = acos( fAngle );

		// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, FString::SanitizeFloat( fAngle ) );

		 if( Hit.ImpactPoint != FVector( 0.0f, 0.0f, 0.0f )  &&
		 	fAngle <= PI / 4.0f )
		{
			GetCharacter()->AddMovementInput( FVector( _fAxisValue, 0.0f, 0.0f ) );
		}
	}
}

void AControllerPlayer::InputGeneralMoveRight( float _fAxisValue )
{
	// Check axis value isn't 0
	if ( _fAxisValue != 0.0f && !this->m_pWidgetInstance->GetShowInventory() && this->m_eGameInputState == EGameInputState::GIS_DEFAULT )
	{
		// Determine forward rotation
		FRotator tRotation{ this->GetControlRotation() };

		// Limit pitch when walking or falling
		APawn* pControlledPawn{ this->GetPawn() };
		if ( pControlledPawn->GetMovementComponent()->IsMovingOnGround() ||
			 pControlledPawn->GetMovementComponent()->IsFalling() )
		{
			tRotation.Pitch = 0.0f;
		}

		// Add movement in right direction
		const FVector tDirection{ FRotationMatrix( tRotation ).GetScaledAxis( EAxis::Y ) };
		
		// m_Destination.Y = ( this->GetPawn()->GetActorLocation().Y + ( 100.0f * _fAxisValue ) );

		FHitResult Hit( ForceInit );
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams( FName( TEXT( "RV_Trace" ) ), true, this );
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->LineTraceSingleByChannel( Hit,
											  GetCharacter()->GetActorLocation() + FVector( 0.0f, 100.0f * _fAxisValue, 100.0f ),
											  GetCharacter()->GetActorLocation() + FVector( 0.0f, 100.0f * _fAxisValue, -500.0f ),
											  ECC_Visibility,
											  RV_TraceParams );

		// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Magenta, Hit.ImpactPoint.ToCompactString() );
		// 
		// DrawDebugLine( GetWorld(), GetCharacter()->GetActorLocation() + FVector( 100.0f * _fAxisValue, 0.0f, 100.0f ),
		// 						   GetCharacter()->GetActorLocation() + FVector( 100.0f * _fAxisValue, 0.0f, -300.0f ),
		// 						   FColor::Red, true, 10.0f );

		float fAngle = FVector::DotProduct( Hit.ImpactNormal, GetCharacter()->GetActorUpVector() );
		fAngle /= ( Hit.ImpactNormal.Size() * GetCharacter()->GetActorUpVector().Size() );
		fAngle = acos( fAngle );

		 if( Hit.ImpactPoint != FVector( 0.0f, 0.0f, 0.0f ) &&
			fAngle <= PI / 4.0f )
		{
			GetCharacter()->AddMovementInput( FVector( 0.0f, _fAxisValue, 0.0f ) );
		}
	}
}

void AControllerPlayer::InputGeneralLookForward( float _fAxisValue )
{
	// Determine the facing direction of the player
	this->m_vFacingVector.X = _fAxisValue;
}

void AControllerPlayer::InputGeneralLookRight( float _fAxisValue )
{
	// Determine the facing direction of the player
	this->m_vFacingVector.Y = _fAxisValue;
}

void AControllerPlayer::InputGeneralInventory()
{
	if ( this->m_eGameInputState != EGameInputState::GIS_CUTSCENE && 
		 this->m_pWidgetInstance != nullptr )
	{
		this->m_pWidgetInstance->ToggleShowInventory();
		this->m_eGameInputState = this->m_pWidgetInstance->GetShowInventory() ? EGameInputState::GIS_INVENTORY : EGameInputState::GIS_DEFAULT;
	}
}

void AControllerPlayer::InputGeneralNavigateUp()
{
	this->m_pWidgetInstance->InputNavigate( UWidgetBase::EMoveDirection::MD_UP );
}

void AControllerPlayer::InputGeneralNavigateDown()
{
	this->m_pWidgetInstance->InputNavigate( UWidgetBase::EMoveDirection::MD_DOWN );
}

void AControllerPlayer::InputGeneralNavigateLeft()
{
	this->m_pWidgetInstance->InputNavigate( UWidgetBase::EMoveDirection::MD_LEFT );
}

void AControllerPlayer::InputGeneralNavigateRight()
{
	this->m_pWidgetInstance->InputNavigate( UWidgetBase::EMoveDirection::MD_RIGHT );
}

void AControllerPlayer::InputGeneralInteract()
{
	ACharacterPlayer* pCharacter{ Cast< ACharacterPlayer >( this->GetCharacter() ) };
	if ( this->m_eGameInputState == EGameInputState::GIS_DEFAULT && pCharacter != nullptr )
	{
		// Get overlapping actors
		TArray< AActor* > arrOverlapList;
		pCharacter->GetPickupArea()->GetOverlappingActors( arrOverlapList );

		// Set initial values for closest
		ASkillInteractable* pClosest{ nullptr };
		float fDistanceToClosest{ MAX_FLT };

		// Find closest actor
		for ( int iActor{ 0 }; iActor < arrOverlapList.Num(); iActor++ )
		{
			ASkillInteractable* pInteractable{ Cast< ASkillInteractable >( arrOverlapList[ iActor ] ) };
			if ( pInteractable != nullptr )
			{
				float fDistance{ ( pInteractable->GetActorLocation() - this->GetCharacter()->GetActorLocation() ).Size() };
				if ( fDistance < fDistanceToClosest )
				{
					pClosest = pInteractable;
					fDistanceToClosest = fDistance;
				}
			}
		}

		// Handle closest actor
		if ( pClosest != nullptr )
		{
			pClosest->Interact( this->m_pInventory );
		}
	}
}

void AControllerPlayer::InputMouseActivateSelected()
{
	if ( this->m_pWidgetInstance != nullptr && this->m_pWidgetInstance->IsMouseOverMenu() )
	{
		this->m_pWidgetInstance->MenuActivateSelected();
	}
}

void AControllerPlayer::InputGeneralHotkey1()
{
	this->HotkeyPressed( 0 );
}

void AControllerPlayer::InputGeneralHotkey2()
{
	this->HotkeyPressed( 1 );
}

void AControllerPlayer::InputGeneralHotkey3()
{
	this->HotkeyPressed( 2 );
}

void AControllerPlayer::InputGeneralHotkey4()
{
	this->HotkeyPressed( 3 );
}

void AControllerPlayer::InputGeneralDrop()
{
	if ( this->m_eGameInputState == EGameInputState::GIS_INVENTORY )
	{
		this->m_pInventory->RemoveFromSlot( this->m_pWidgetInstance->GetInventoryIndex() );
	}
}

void AControllerPlayer::InputGeneralToggleDebug()
{
	USoulsGameInstance* pInstance{ Cast< USoulsGameInstance >( this->GetGameInstance() ) };
	if ( pInstance != nullptr )
	{
		pInstance->ToggleDebugScreen();
	}
}