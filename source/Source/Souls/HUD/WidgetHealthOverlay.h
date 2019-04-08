// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Blueprint/UserWidget.h"

// Library Includes

// Local Includes

//Generated Include
#include "WidgetHealthOverlay.generated.h"

// Prototypes
UCLASS( Blueprintable )
class UWidgetHealthOverlay : public UUserWidget
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	float GetDesired() const;
	void SetDesired( const float& _krfDesired );

	float GetCurrent() const;
	void SetCurrent( const float& _krfCurrent );

	UFUNCTION( BlueprintPure, Category = "Souls" )
		ESlateVisibility GetOverlayVisibility() const;

	UFUNCTION( BlueprintPure, Category = "Souls" )
		float GetBarLeft() const;

	UFUNCTION( BlueprintPure, Category = "Souls" )
		float GetBarRight() const;

	UFUNCTION( BlueprintPure, Category = "Souls" )
		float GetBarRightPercent() const;

	// UE4 overrides
	virtual void NativeTick( const FGeometry& _tGeometry, float _fDeltaTime ) override;
	
protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls" ) float m_fTimeScale;

protected:
private:
	UPROPERTY() float m_fDesired;
	UPROPERTY() float m_fCurrent;

	UPROPERTY() float m_fLeft;
	UPROPERTY() float m_fRight;
	UPROPERTY() float m_fRightPercent;
};