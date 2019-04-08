// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes

//Generated Include
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FInventoryDropDelegate, class USkillBase*, _pSkill );

// Prototypes
UCLASS( Blueprintable )
class UInventory : public UObject
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	int32 GetHotBarSlotCount() const;
	int32 GetStorageSlotCount() const;

	class UInventorySlot* GetSlot( const int32& _kriSlotNumber ) const;
	class USkillBase* GetSlotContents( const int32& _kriSlotNumber ) const;

	class AControllerPlayer* GetOwningController() const;
	class UCharacteristics* GetOwningCharacteristics() const;
	class UAnimInstancePlayer* GetOwningAnimInstance() const;

	FInventoryDropDelegate& GetOnDropSkillDelegate();

	// Queries
	bool IsSlotOccupied( const int32& _kriSlotNumber ) const;
	bool CanAddSkill() const;
	int FirstUnoccupiedSlotNumber() const;

	// Other
	void Initialise( const int& _kriHotBarSlotCount, const int& _kriStorageSlotCount, class AControllerPlayer* _pController, class UCharacteristics* _pCharacteristics, class UAnimInstancePlayer* _pAnimInstance = nullptr );
	bool AddSkill( class USkillBase* _pSkill );
	bool ForceAddSkill( const int32& _kriSlotNumber, class USkillBase* _pSkill );
	bool SwapSkill( const int32& _kriSlotNumberA, const int32& _kriSlotNumberB );
	bool RemoveSkill( class USkillBase* _pSkill, const bool _bDrop = true );
	bool RemoveFromSlot( const int32& _kriSlotNumber, const bool _bDrop = true );
	void RemoveAll( const bool _bDrop = true );
	
	bool AttachWidgetIcons( TArray< class UWidgetInventoryIcon* >& _rarrIcons );

	bool Process( const float& _krfDeltaTime );

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() int32 m_iHotBarSlotCount;
	UPROPERTY() int32 m_iStorageSlotCount;
	UPROPERTY() class AControllerPlayer* m_pOwningController;
	UPROPERTY() class UCharacteristics* m_pOwningCharacteristics;
	UPROPERTY() class UAnimInstancePlayer* m_pOwningAnimInstance;
	UPROPERTY() TArray< class UInventorySlot* > m_arrSlots;

	FInventoryDropDelegate m_tOnDropSkill;
};