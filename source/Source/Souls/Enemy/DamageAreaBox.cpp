// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes

// This Include
#include "DamageAreaBox.h"

// Implementation

UDamageAreaBox::UDamageAreaBox( const FObjectInitializer& ObjectInitializer )
	: UBoxComponent( ObjectInitializer )
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	this->OnComponentBeginOverlap.AddDynamic( this, &UDamageAreaBox::OnOverlapBegin );
}

// Called when the game starts
void UDamageAreaBox::InitializeComponent()
{
	this->m_bActive = false;
}

void UDamageAreaBox::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	if( m_fCoolDown > 0.0f )
	{
		m_fCoolDown -= DeltaTime;

		if( m_bActive && m_fCoolDown <= 0.0f )
		{
			TArray< AActor* >AllOverlaps;

			this->GetOverlappingActors( AllOverlaps );

			if( AllOverlaps.Num() == 0 )
			{
				m_bActive = false;
			}
			else
			{
				for( int32 Idx = 0; Idx < AllOverlaps.Num(); ++Idx )
				{
					FDamageEvent Event;

					AllOverlaps[ Idx ]->TakeDamage( DamageAmount, Event, this->GetAttachmentRootActor()->GetInstigatorController(), this->GetAttachmentRootActor() );
				}

				m_fCoolDown = CoolDown;
			}
		}
	}


}

void UDamageAreaBox::OnOverlapBegin( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	m_bActive = true;

	if( m_fCoolDown <= 0.0f )
	{
		FDamageEvent Event;
		Event.DamageTypeClass = DamageType;

		OtherActor->TakeDamage( DamageAmount, Event, this->GetAttachmentRootActor()->GetInstigatorController(), this->GetAttachmentRootActor() );

		m_fCoolDown = CoolDown;
	}
}