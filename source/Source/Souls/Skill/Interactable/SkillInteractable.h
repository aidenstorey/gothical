// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "GameFramework/Actor.h"

// Library Includes

// Local Includes

//Generated Include
#include "SkillInteractable.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ASkillInteractable : public AActor
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	ASkillInteractable( const FObjectInitializer& _krtInitialiser );

	// Accesors
	class UCapsuleComponent* GetInteractArea() const;
	class USkillBase* GetSkill() const;

	bool HasOverlay() const;
	void AddOverlay( class UWidgetInteractableOverlay* _pOverlay );

	// Other
	bool Initialise( class USkillBase* _pSkill );
	bool Interact( class UInventory* _pInventory );

protected:
	// Other
	virtual bool ReceiveInitialise( class USkillBase* _pSkill );
	virtual bool ReceiveInteract( class UInventory* _pInventory );

private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls" ) FVector m_vHoverOffset;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls" ) FString m_tHoverString;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Souls) class UCapsuleComponent* m_pInteractArea;
	UPROPERTY() float m_fInteractRadius;
	UPROPERTY() float m_fInteractHalfHeight;
	UPROPERTY() class USkillBase* m_pSkill;


private:

};