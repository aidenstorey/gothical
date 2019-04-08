// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes

//Generated Include
#include "InventorySlot.generated.h"

// Prototypes
UCLASS( Blueprintable )
class UInventorySlot : public UObject
{
	GENERATED_BODY()

	// Member Types
public:
	UENUM() 
	enum class ESlotType : uint8
	{
		ST_STORAGE,
		ST_HOTBAR,
	};

	UENUM()
	enum class ESlotState : uint8
	{
		SS_UNOCCUPIED,
		SS_OCCUPIED,
	};
	
	// Member Functions
public:
	// Accessors
	ESlotType GetType() const;
	void SetType( const ESlotType& _kreType );

	ESlotState GetState() const;
	class USkillBase* GetSkill() const;
	void SetSkill( class USkillBase* _pSkill );

	class UInventory* GetOwningInventory() const;
	void SetOwningInventory( class UInventory* _pInventory );

	class UWidgetInventoryIcon* GetIcon() const;
	void SetIcon( class UWidgetInventoryIcon* _pIcon );

	// Queries
	bool IsHotBar() const;
	bool IsUnoccupied() const;
	bool HasSkill( class USkillBase* _pSkill ) const;

	// Other
	bool AddSkill( class USkillBase* _pSkill );
	class USkillBase* ReplaceSkill( class USkillBase* _pSkill );
	class USkillBase* RemoveSkill();
	bool SwapSkill( UInventorySlot* _pOtherSlot );

	bool Process( const float& _krfDeltaTime );

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() ESlotType m_eType;
	UPROPERTY() ESlotState m_eState;
	UPROPERTY() class USkillBase* m_pSkill;
	UPROPERTY() class UInventory* m_pOwningInventory;
	UPROPERTY() class UWidgetInventoryIcon* m_pIcon;

};