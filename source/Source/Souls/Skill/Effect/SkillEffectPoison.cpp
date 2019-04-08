// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Enemy/Characters/EnemyCharacter.h"
#include "../../Enemy/Controllers/EnemyControllerBase.h"

//This Include
#include "SkillEffectPoison.h"

// Implementation
ASkillEffectPoison::ASkillEffectPoison( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser )
{
	this->m_pDamageArea = _krtInitialiser.CreateDefaultSubobject< USphereComponent >( this, "DamageArea" );
	this->RootComponent = this->m_pDamageArea;
	this->m_pDamageArea->SetCollisionProfileName( FName( "PoisonDamageArea" ) );
	this->m_pDamageArea->OnComponentBeginOverlap.AddDynamic( this, &ASkillEffectPoison::OnOverlapBegin );

	this->PrimaryActorTick.bCanEverTick = true;
}

void ASkillEffectPoison::Initialise( float _fLifeTime, float _fRadius, float _fPoisonDuration, float _fPoisonRate, int32 _iPoisonAmount )
{
	this->m_fLifeTime = _fLifeTime;
	this->m_fRadius = _fRadius;
	this->m_fPoisonDuration = _fPoisonDuration;
	this->m_fPoisonRate = _fPoisonRate;
	this->m_iPoisonAmount = _iPoisonAmount;

	this->m_pDamageArea->SetSphereRadius( _fRadius, true );
}


void ASkillEffectPoison::Tick( float _fDeltaTime )
{
	this->m_fLifeTime -= _fDeltaTime;
	if ( this->m_fLifeTime <= 0.0f )
	{
		this->Destroy();
	}
}

void ASkillEffectPoison::OnOverlapBegin( class AActor* _OtherActor, class UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult )
{
	auto pEnemy = Cast< AEnemyControllerBase >( Cast< AEnemyCharacter >( _OtherActor )->GetController() );
	if ( pEnemy != nullptr )
	{
		pEnemy->GetCharacteristics()->Poison( this->m_fPoisonDuration, this->m_fPoisonRate, this->m_iPoisonAmount );
	}
}