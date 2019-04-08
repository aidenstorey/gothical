// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Characteristics.h"
#include "../../Player/Character/CharacterPlayer.h"
#include "../../Player/Animation/AnimInstancePlayer.h"
#include "../../Player/Animation/AnimStatePlayer.h"
#include "../../Player/Other/ProjectileFireball.h"

//This Include
#include "SkillFireball.h"

// Implementation
USkillFireball::USkillFireball()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillFireball'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillFireball::ReceiveInitialise()
{
	this->m_tDisplayName = FText::FromString( "Fireball" );

	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Default" };
		auto pData =  this->m_pSkillStatsDataTable->FindRow< FSkillDataFireball >( FName( *tName ), TEXT( "LookUpOperation" ) );
		if ( pData != nullptr )
		{
			this->m_pProjectileClass = pData->m_pProjectileClass;
			this->m_fSpeed = pData->m_fSpeed;
			this->m_fRange = pData->m_fRange;
			this->m_fInitialDamageMultiplyer = pData->m_fInitialDamageMultiplyer;
			this->m_fExplosionRadius = pData->m_fExplosionRadius;
			this->m_fExplosionDamageMultiplyer = pData->m_fExplosionDamageMultiplyer;
			this->m_iExplosionMaxTargets = pData->m_iExplosionMaxTargets;
			this->m_bExplosionDamageFalloff = pData->m_bExplosionDamageFalloff;
		}
	}
}

bool USkillFireball::ReceiveActivate()
{
	bool bReturn{ false };

	if ( this->m_pOwningController != nullptr && this->m_pOwningController->ActivateSkill( this ) )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_FIREBALL );
		bReturn = true;
	}


	return ( bReturn );
}

bool USkillFireball::ReceiveDeactivate()
{
	bool bReturn{ false };

	if ( this->m_pOwningController != nullptr )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_IDLE_WALK_RUN );
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillFireball::ReceiveTrigger()
{
	bool bReturn{ false };

	if ( this->m_pOwningController != nullptr && !this->OnCooldown() )
	{
		ACharacterPlayer* pOwningCharacter{ Cast< ACharacterPlayer >( this->m_pOwningController->GetCharacter() ) };
		if ( pOwningCharacter != nullptr )
		{
			FVector vLocation{ pOwningCharacter->GetActorLocation() };
			FRotator vRotator{ pOwningCharacter->GetActorRotation() };
			auto pMeshComponent = this->m_pOwningController->GetAnimInstancePlayer()->GetSkelMeshComponent();

			auto pSocket = pMeshComponent->GetSocketByName( FName( "Delilah_rigLPalm_socket" ) );
			if ( pSocket != nullptr )
			{
				vLocation = pSocket->GetSocketLocation( pMeshComponent );
			}

			auto pTempProjectile = Cast< AProjectileFireball >( UGameplayStatics::BeginSpawningActorFromClass( pOwningCharacter, this->m_pProjectileClass, FTransform( vRotator, vLocation + ( pOwningCharacter->GetActorForwardVector() * 10.0f ) ) ) );
			if ( pTempProjectile )
			{
				float fLifeTime{ static_cast< float >( this->m_pOwningController->GetCharacteristics()->GetRange() ) / this->m_fSpeed };
				int32 iDamage{ this->m_pOwningController->GetCharacteristics()->GetDamage() * static_cast< int32 >( m_fInitialDamageMultiplyer ) };
				
				UGameplayStatics::FinishSpawningActor( pTempProjectile, FTransform( vRotator, vLocation + ( pOwningCharacter->GetActorForwardVector() * 10.0f ) ) );

				pTempProjectile->Initialize( this->m_pOwningController, this->m_fSpeed, iDamage, fLifeTime );
				pTempProjectile->InitialiseExplosion( this->m_fExplosionRadius, this->m_fExplosionDamageMultiplyer, this->m_iExplosionMaxTargets, this->m_bExplosionDamageFalloff );

			}

			if ( this->m_pTriggerParticles != nullptr )
			{
				UGameplayStatics::SpawnEmitterAttached( this->m_pTriggerParticles,
														pMeshComponent,
														FName( "Delilah_rigLPalm_socket" ) );
			}

			this->m_fTimeRemaining = this->m_fCooldown;
		}
	}

	return ( bReturn );
}
