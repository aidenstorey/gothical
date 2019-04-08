// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Blueprint/UserWidget.h"

// Library Includes

// Local Includes
#include "WidgetState.h"

//Generated Include
#include "WidgetMenuButton.generated.h"

// Types
UENUM( Blueprintable )
enum class EMenuButtonState
{
	BS_NORMAL,
	BS_HOVER,
	BS_CLICKED,
};

// Prototypes
UCLASS( Blueprintable )
class UWidgetMenuButton : public UUserWidget
{
	GENERATED_BODY()

	// Member Types
public:

protected:
private:

	// Member Functions
public:
	// Accessors
	UFUNCTION( BlueprintPure, Category = "Souls" ) TEnumAsByte< EMenuButtonState > GetButtonState() const;
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void SetButtonState( TEnumAsByte< EMenuButtonState > _eState );

	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility GetNormalVisibility() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility GetHoverVisibility() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility GetClickedVisibility() const;

	UFUNCTION( BlueprintPure, Category = "Souls" ) UTexture2D* GetNormalImage() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) UTexture2D* GetHoverImage() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) UTexture2D* GetClickedImage() const;

	class UWidgetBase* GetOwningWidget() const;
	void SetOwningWidget( class UWidgetBase* _pWidget );

	// Other
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void MouseFocus();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void MouseUnfocus();


protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Menu Index" ) ) int32 m_iMenuIndex;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Change to State" ) ) TEnumAsByte< EWidgetState > m_eChangeToState;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Normal Image" ) ) UTexture2D* m_pNormalImage;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Hover Image" ) ) UTexture2D* m_pHoverImage;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Clicked Image" ) ) UTexture2D* m_pClickedImage;

protected:
private:
	TEnumAsByte< EMenuButtonState > m_eButtonState;
	UPROPERTY() class UWidgetBase* m_pOwningWidget;

};