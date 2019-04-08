// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Player/WinCondition.h"
#include "../SoulsGameMode.h"

// This Include
#include "AreaLock.h"

// Implementation

// Sets default values
AAreaLock::AAreaLock( const FObjectInitializer& ObjectInitialiser )
{
	this->m_TriggerBox = ObjectInitialiser.CreateAbstractDefaultSubobject< UBoxComponent >( this, TEXT( "TriggerBox" ) );
	this->m_TriggerBox->InitBoxExtent( FVector( 50.0f, 50.0f, 50.0f ) );
	this->m_TriggerBox->AttachParent = this->RootComponent;

	this->m_TriggerBox->OnComponentBeginOverlap.AddDynamic( this, &AAreaLock::UnlockArea );
}

void AAreaLock::Initialise( const EWinCondition& _kreUnlockCondition )
{
	m_eUnlockCondition = _kreUnlockCondition;
}

void AAreaLock::UnlockArea( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult )
{
	// Check if the unlock condition has been met
	if( Cast< ASoulsGameMode >( UGameplayStatics::GetGameMode( this->GetWorld() ) )->CheckWinConditionMet( m_eUnlockCondition ) )
	{

	}
}