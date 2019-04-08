// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Animation/AnimInstance.h"

// Library Includes

// Local Includes
#include "AnimStatePlayer.h"

//Generated Include
#include "AnimInstancePlayer.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAnimStateChange, EAnimStatePlayer, _eState );

// Prototypes
UCLASS( Blueprintable )
class UAnimInstancePlayer : public UAnimInstance
{
	GENERATED_BODY()

	// Member Functions
public:
	// Accessors
	UFUNCTION( BlueprintPure, Category = "Souls" ) EAnimStatePlayer GetState();
	UFUNCTION( BlueprintCallable, Category = "Souls" ) EAnimStatePlayer SetState( EAnimStatePlayer _eState );
	UFUNCTION( BlueprintPure, Category = "Souls" ) float GetAngle();
	void SetAngle( float _fAngle );

	FAnimStateChange& GetOnStateChangeDelegate();

protected:
private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( DisplayName = "Animation State" ) ) EAnimStatePlayer m_eState;

protected:
private:
	UPROPERTY() float m_fAngle;
	FAnimStateChange m_tOnStateChange;

};