// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "Enemy/Controllers/Bosses/BossControllerBase.h"

// Generated Includes
#include "BossReaperController.generated.h"

// Prototypes

UCLASS()
class SOULS_API ABossReaperController : public ABossControllerBase
{
	GENERATED_BODY()
	
	// Member Functions
public:
	// Constructor
	ABossReaperController( const FObjectInitializer& _krtInitialiser );

	// Overridden from ABossControllerBase
	virtual void ProcessState() override;
	virtual FVector GetGoToLocation() override;

	// Reaper Attacks
	void Reap();
	void Rend();
	void Gravitate();
	void Harvest();

protected:
	// Other
	virtual void ReceiveDeath() override;

private:	
	
	// Member Variables
public:
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Boss" )
	float m_fDragStrength = 50.0f;

protected:
private:

};
