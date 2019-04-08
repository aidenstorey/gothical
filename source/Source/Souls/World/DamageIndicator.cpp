// Fill out your copyright notice in the Description page of Project Settings.

#include "Souls.h"
#include "DamageIndicator.h"


// Sets default values
ADamageIndicator::ADamageIndicator( const FObjectInitializer& ObjectInitializer )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;

	this->m_pDamageText = CreateDefaultSubobject< UTextRenderComponent >( TEXT( "TextRender" ) );
	this->m_pDamageText->SetRelativeScale3D( FVector( 1.5f, 1.5f, 1.5f ) );
	this->m_pDamageText->SetText( FText::AsNumber( 0 ) );
	this->m_pDamageText->AttachTo( this->GetRootComponent() );
	// this->m_pDamageText->SetRelativeRotation( FRotator( 0.0f, 180.0f, 0.0f ) );

	this->InitialLifeSpan = 0.5f;
}

void ADamageIndicator::Initialise( const float& _krfDamageAmount )
{
	this->m_pDamageText->SetText( FText::AsNumber( _krfDamageAmount ) );
}

// Called when the game starts or when spawned
void ADamageIndicator::BeginPlay()
{
	Super::BeginPlay();

	this->SetActorRotation( UGameplayStatics::GetPlayerCameraManager( this, 0 )->GetCameraRotation() );
	this->AddActorLocalRotation( FRotator( 0.0f, 180.0f, 0.0f ) );
}

// Called every frame
void ADamageIndicator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	this->AddActorWorldOffset( FVector( 0.0f, 0.0f, 500.0f * DeltaTime ) );
}

