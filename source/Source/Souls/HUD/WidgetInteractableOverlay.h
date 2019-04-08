// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Blueprint/UserWidget.h"

// Library Includes

// Local Includes

//Generated Include
#include "WidgetInteractableOverlay.generated.h"

// Prototypes
UCLASS( Blueprintable )
class UWidgetInteractableOverlay : public UUserWidget
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	UFUNCTION( BlueprintPure, Category = "Souls" ) FVector GetWorldPosition() const;
	UFUNCTION( BlueprintPure, Category = "Souls" ) FString GetText() const;

	void SetOwner( class APlayerController* _pOwner );
	
	// Other
	void Initialise( FVector _vWorldPosition, FString _tText );

	void UpdatePosition();

	static void SetButtonName( FString _tButtonName );


protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() class APlayerController* m_pOwner;
	UPROPERTY() FVector m_vWorldPosition;
	UPROPERTY() FString m_tText;

	static FString m_tButtonName;
};