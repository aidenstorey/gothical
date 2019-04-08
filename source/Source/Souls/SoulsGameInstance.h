// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Engine/GameInstance.h"

// Library Includes

// Local Includes

//Generated Include
#include "SoulsGameInstance.generated.h"

// Prototypes
UCLASS()
class USoulsGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USoulsGameInstance();

	// Accessors
	bool GetIsStartUp() const;
	void SetIsStartUp( const bool& _krbStartUp );

	bool GetShowDebugScreen() const;
	void SetShowDebugScreen( const bool& _krbShow );

	class USkillManager* GetSkillManager();

	// UE4 Overrides
	virtual void Init() override;

	// Other
	void ToggleDebugScreen();

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() bool m_bIsStartUp;
	UPROPERTY() bool m_bShowDebugScreen;
	UPROPERTY() class USkillManager* m_pSkillManager;

};