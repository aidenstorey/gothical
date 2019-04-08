// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "SkillInteractable.h"

//Generated Include
#include "SkillSpire.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ASkillSpire : public ASkillInteractable
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	ASkillSpire( const FObjectInitializer& _krtInitialiser );

	// UE4 Override
	virtual void BeginPlay() override;
	virtual void Tick( float _fDeltaTime ) override;

	// Other
	UFUNCTION( BlueprintImplementableEvent, Category = "Souls" ) void InteractSuccessful();

	// Bound
	UFUNCTION() void OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult );
	UFUNCTION() void OnOverlapEnd( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

protected:
	// Other 
	virtual bool ReceiveInteract( class UInventory* _pInventory ) override;

private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls" ) TSubclassOf< class UWidgetInteractableOverlay > m_pOverlayClass;

protected:
	UPROPERTY() bool m_bShowOverlay;
	UPROPERTY() class UWidgetInteractableOverlay* m_pOverlayInstance;

private:

};