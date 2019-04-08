// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Inventory/Inventory.h"
#include "../../SoulsGameInstance.h"
#include "../../HUD/WidgetBase.h"
#include "../../HUD/WidgetInteractableOverlay.h"
#include "../../Player/Character/CharacterPlayer.h"
#include "../../Player/Controller/ControllerPlayer.h"
#include "../SkillManager.h"

//This Include
#include "SkillSpire.h"

// Implementation
ASkillSpire::ASkillSpire( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	this->m_fInteractRadius = 200.0f;
	this->m_fInteractHalfHeight = 200.0f;
	this->m_tHoverString = " to interact";

	this->PrimaryActorTick.bCanEverTick = true;
}

void ASkillSpire::BeginPlay()
{
	Super::BeginPlay();

	auto pGameInstance = Cast< USoulsGameInstance >( this->GetGameInstance() );
	if ( pGameInstance != nullptr )
	{
		if ( !this->Initialise( pGameInstance->GetSkillManager()->RequestSkill() ) )
		{
			this->Initialise( pGameInstance->GetSkillManager()->CreateSkill( ESkillClassType::ST_PASSIVE ) );
		}
	}

	if ( this->m_pOverlayClass != nullptr )
	{
		this->m_pOverlayInstance = CreateWidget< UWidgetInteractableOverlay >( this->GetGameInstance(), this->m_pOverlayClass );
		this->m_pOverlayInstance->Initialise( this->GetActorLocation() + this->m_vHoverOffset, this->m_tHoverString );
	}

	m_pInteractArea->OnComponentBeginOverlap.AddDynamic( this, &ASkillSpire::OnOverlapBegin );
	m_pInteractArea->OnComponentEndOverlap.AddDynamic( this, &ASkillSpire::OnOverlapEnd );
}

void ASkillSpire::Tick( float _fDeltaTime )
{
	if ( this->m_bShowOverlay )
	{
		this->m_pOverlayInstance->UpdatePosition();
	}
}

bool ASkillSpire::ReceiveInteract( UInventory* _pInventory )
{
	bool bReturn{ true };

	if ( !_pInventory->AddSkill( this->m_pSkill ) )
	{
		bReturn = false;
	}
	else
	{
		this->InteractSuccessful();
		this->m_pSkill = nullptr;
		this->m_pOverlayInstance->SetVisibility( ESlateVisibility::Hidden );
	}

	return ( bReturn );
}

void ASkillSpire::OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult )
{
	auto pController = Cast< AControllerPlayer >( Cast< ACharacterPlayer >( _OtherActor )->GetController() );
	if ( pController != nullptr && this->m_pSkill != nullptr )
	{
		this->m_bShowOverlay = true;		
		this->m_pOverlayInstance->SetOwner( pController );
		this->m_pOverlayInstance->UpdatePosition();
		this->m_pOverlayInstance->SetVisibility( ESlateVisibility::Visible );
	}
}

void ASkillSpire::OnOverlapEnd( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	auto pController = Cast< AControllerPlayer >( Cast< ACharacterPlayer >( OtherActor )->GetController() );
	if ( pController != nullptr )
	{
		this->m_bShowOverlay = false;
		this->m_pOverlayInstance->SetOwner( nullptr );
		this->m_pOverlayInstance->SetVisibility( ESlateVisibility::Hidden );
	}
}