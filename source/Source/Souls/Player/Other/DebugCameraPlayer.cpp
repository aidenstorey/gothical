// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../Controller/ControllerPlayer.h"

//This Include
#include "DebugCameraPlayer.h"

// Implementation
void ADebugCameraPlayer::BeginPlay()
{
	this->InputComponent->BindAction( "K_DebugTeleport", EInputEvent::IE_Pressed, this, &ADebugCameraPlayer::TeleportPlayer );
}

void ADebugCameraPlayer::TeleportPlayer()
{
	auto pPlayer = Cast< AControllerPlayer >( this->OriginalControllerRef );
	if ( pPlayer != nullptr )
	{
		FVector tOutLocation;
		FRotator tOutRotation;
		this->GetActorEyesViewPoint( tOutLocation, tOutRotation );
		pPlayer->Teleport( tOutLocation );
	}
}