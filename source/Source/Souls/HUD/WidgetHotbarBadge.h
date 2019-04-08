// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Blueprint/UserWidget.h"

// Library Includes

// Local Includes
#include "InputData.h"

//Generated Include
#include "WidgetHotbarBadge.generated.h"

// Prototypes
UCLASS( Blueprintable )
class UWidgetHotbarBadge : public UUserWidget
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	UFUNCTION( BlueprintPure, Category = "Accessors" ) TEnumAsByte< EInputMethod > GetInputMehtod();
	UFUNCTION( BlueprintCallable, Category = "Accessors" ) void SetInputMethod( TEnumAsByte< EInputMethod > _eMethod );

	UFUNCTION( BlueprintPure, Category = "Accessors" ) bool GetShowInventory();
	UFUNCTION( BlueprintCallable, Category = "Accessors" ) void SetShowInventory( bool _bShow );

	UFUNCTION( BlueprintPure, Category = "Accessors" ) class USkillBase* GetSkill();
	UFUNCTION( BlueprintCallable, Category = "Accessors" ) void SetSkill( class USkillBase* _pSkill );

	UFUNCTION( BlueprintPure, Category = "Icons" ) class UTexture2D* GetHealthIcon();
	UFUNCTION( BlueprintPure, Category = "Icons" ) class UTexture2D* GetDamageIcon();
	UFUNCTION( BlueprintPure, Category = "Icons" ) class UTexture2D* GetAttackSpeedIcon();
	UFUNCTION( BlueprintPure, Category = "Icons" ) class UTexture2D* GetMovementSpeedIcon();
	UFUNCTION( BlueprintPure, Category = "Icons" ) class UTexture2D* GetActivateButtonIcon();

	UFUNCTION( BlueprintPure, Category = "Icons" ) ESlateVisibility DetermineHealthIconVisibility();
	UFUNCTION( BlueprintPure, Category = "Icons" ) ESlateVisibility DetermineDamageIconVisibility();
	UFUNCTION( BlueprintPure, Category = "Icons" ) ESlateVisibility DetermineAttackSpeedIconVisibility();
	UFUNCTION( BlueprintPure, Category = "Icons" ) ESlateVisibility DetermineMovementSpeedIconVisibility();
	UFUNCTION( BlueprintPure, Category = "Icons" ) ESlateVisibility DetermineActivateIconVisibility();

protected:
private:

	// Member Variables
public:
protected:
	UPROPERTY() TEnumAsByte< EInputMethod > m_eInputMethod;
	UPROPERTY() bool m_bShowInventory;
	UPROPERTY() class USkillBase* m_pSkill;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Health Icon" ) ) class UTexture2D* m_pHealthIcon;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Damage Icon" ) ) class UTexture2D* m_pDamageIcon;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Attack Speed Icon" ) ) class UTexture2D* m_pAttackSpeedIcon;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Movement Speed Icon" ) ) class UTexture2D* m_pMovementSpeedIcon;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Activate Button Icon (Keyboard)" ) ) class UTexture2D* m_pActivateButtonIconKeyboard;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Activate Button Icon (Controller)" ) ) class UTexture2D* m_pActivateButtonIconController;


private:

};