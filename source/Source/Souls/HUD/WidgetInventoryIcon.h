// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Blueprint/UserWidget.h"

// Library Includes

// Local Includes

//Generated Include
#include "WidgetInventoryIcon.generated.h"

// Prototypes
UCLASS( Blueprintable )
class UWidgetInventoryIcon : public UUserWidget
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	bool GetSelected() const;
	void SetSelected( bool _bSelected );

	class USkillBase* GetSkill() const;
	void SetSkill( class USkillBase* _pSkill );

	void SetBadge( class UWidgetHotbarBadge* _pBadge );

	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility GetSelectedEmptyVisibility() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) ESlateVisibility GetSelectedIconVisibility() const;

	UFUNCTION( BlueprintPure, Category = "Souls" ) class UTexture2D* GetSkillTexture() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) float GetSkillCooldownPercentage() const;

	UFUNCTION( BlueprintPure, Category = "Souls" ) FLinearColor GetColorAndOpacity() const;
	
	class UWidgetBase* GetOwningWidget() const;
	void SetOwningWidget( class UWidgetBase* _pWidget );

	// Other
	UFUNCTION( BlueprintCallable, Category = "Souls" ) void MouseFocus();

protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Row Position" ) ) int32 m_iRowPosition;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Col Position" ) ) int32 m_iColPosition;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Passive Icons" ) ) TArray< class UWidget* > m_arrPassiveIcons;

protected:
private:
	UPROPERTY() bool m_bSelected;
	UPROPERTY() class USkillBase* m_pSkill;
	UPROPERTY() class UWidgetBase* m_pOwningWidget;

	UPROPERTY() class UWidgetHotbarBadge* m_pBadge;

};