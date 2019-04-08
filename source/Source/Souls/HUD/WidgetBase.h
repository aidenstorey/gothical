// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Blueprint/UserWidget.h"

// Library Includes

// Local Includes
#include "WidgetState.h"

//Generated Include
#include "WidgetBase.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FStateChange, EWidgetState, _eIncomingState, EWidgetState, _eCurrentState );

// Prototypes
UCLASS( Blueprintable )
class UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	// Member Types
public:
	UENUM()
	enum class EMoveDirection : uint8
	{
		MD_UP,
		MD_DOWN,
		MD_LEFT,
		MD_RIGHT,
	};

protected:
private:

	// Member Functions
public:
	// Accessors
	UFUNCTION( BlueprintPure, Category = "Souls" ) EWidgetState GetState() const;
	void SetState( const EWidgetState& _kreState );

	UFUNCTION( BlueprintPure, Category = "Souls" ) FText GetSelectedName();
	UFUNCTION( BlueprintPure, Category = "Souls" ) FText GetSelectedDescription();

	UFUNCTION( BlueprintPure, Category = "Souls" ) bool GetShowDebugText();
	UFUNCTION( BlueprintPure, Category = "Souls" ) FText GetDebugScreenText();

	UFUNCTION( BlueprintCallable, Category = "Souls" ) void SetSelected( int32 _iRow, int32 _iCol );
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void SetMenuSelected( int32 _iOption, TArray< class UWidgetMenuButton* >& _rarrMenuButtons );

	UFUNCTION( BlueprintCallable, Category = "Souls" ) ESlateVisibility GetDebugScreenVisibility();
	
	bool GetShowInventory() const;
	void SetShowInventory( const bool& _krbShow );
	void ToggleShowInventory();

	int GetInventoryIndex() const;
	
	UFUNCTION( BlueprintPure, Category = "Souls" ) TEnumAsByte< EInputMethod > GetInputMethod() const;
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void ToggleInputMethod();
	void SetInputMethod( TEnumAsByte< EInputMethod > _eInputMethod );

	UFUNCTION( BlueprintPure, Category = "Souls" ) float GetHealthMin() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) float GetHealthMax() const;

	FStateChange& GetOnStateChangeDelegate();
		
	// Query 
	bool IsInMenu();
	bool IsMouseOverMenu();

	// Bound
	UFUNCTION() void SetHealth( float _fPercent );
	UFUNCTION() void SetHealthDesired( float _fPercent );

	// Other
	void Initialise( class AControllerPlayer* _pOwner );

	UFUNCTION( BlueprintCallable, Category = "Souls" ) void ChangeState( EWidgetState _eState );
	UFUNCTION( BlueprintCallable, Category = "Souls" ) ESlateVisibility DetermineVisibility( EWidgetState _eCheck, ESlateVisibility _eTrue, ESlateVisibility _eFalse );
	UFUNCTION( BlueprintCallable, Category = "Souls" ) ESlateVisibility DetermineInventoryVisibility();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) ESlateVisibility DetermineInputConfigVisibility( EInputMethod _eInputMethod );
	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility DetermineControllerButtonVisibility();
	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility DetermineKeyboardButtonVisibility();

	void InputPause();
	void InputBack();
	void InputEscape();

	void InputNavigate( EMoveDirection _eDirection );

	void InventorySelectedMove( EMoveDirection _eDirection );
	void InventorySelectedReset();

	void MenuSelectedMouse( int32 _iSelected );
	void MenuDeselectMouse();
	void MenuSelectedMove( EMoveDirection _eDirection, TArray< class UWidgetMenuButton* >& _rarrMenuButtons );
	void MenuActivateSelected();
	void MenuSelectedReset();
	
	void Process( float _fDeltaTime );
	
protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Feedback Link" ) ) FString m_tFeedbackLink;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Inventory Row Count" ) ) int32 m_iInventoryRowCount;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Inventory Col Count" ) ) int32 m_iInventoryColCount;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Inventory Icons" ) ) TArray< class UWidgetInventoryIcon* > m_arrInventoryIcons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Main Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrMainMenuButtons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Pause Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrPauseMenuButtons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Death Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrDeathMenuButtons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Win Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrWinMenuButtons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Controls Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrControlsMenuButtons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Graphics Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrGraphicsMenuButtons;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Credits Menu Buttons" ) ) TArray< class UWidgetMenuButton* > m_arrCreditsButtons;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Hotbar Badges" ) ) TArray< class UWidgetHotbarBadge* > m_arrHotbarBadges;

protected:
private:
	UPROPERTY() TEnumAsByte< EInputMethod > m_eInputMethod;
	UPROPERTY() EWidgetState m_eState;
	UPROPERTY() FStateChange m_tOnStateChange;
	UPROPERTY() class AControllerPlayer* m_pOwner;
	UPROPERTY() class UInventory* m_pInventory;
	UPROPERTY() bool m_bShowInventory;
	UPROPERTY() int32 m_iInventoryRowSelected;
	UPROPERTY() int32 m_iInventoryColSelected;
	UPROPERTY() int32 m_iInventoryIndex;
	UPROPERTY() int32 m_iMenuIndex;
	UPROPERTY() bool m_bMouseOver;

	UPROPERTY() float m_bOnInputCooldown;
	UPROPERTY() float m_fInputCooldownDuration;
	UPROPERTY() float m_fInputTimeElapsed;

	UPROPERTY() bool m_bMinDepleting;
	UPROPERTY() bool m_bMaxDepleting;
	UPROPERTY() float m_fHealthDesired;
	UPROPERTY() float m_fHealthMin;
	UPROPERTY() float m_fHealthMax;
};