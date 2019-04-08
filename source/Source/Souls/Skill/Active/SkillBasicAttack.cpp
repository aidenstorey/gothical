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
#include "../../Player/Other/ProjectilePlayer.h"

//This Include
#include "SkillBasicAttack.h"

// Implementation
USkillBasicAttack::USkillBasicAttack()
{
	ConstructorHelpers::FObjectFinder< UDataTable > pSkillData{ TEXT( "DataTable'/Game/Data/SkillBasicAttack'" ) };
	if ( pSkillData.Succeeded() )
	{
		this->m_pSkillStatsDataTable = pSkillData.Object;
	}
}

void USkillBasicAttack::ReceiveInitialise()
{
	this->m_tDisplayName = FText::FromString( "BasicAttack" );

	if ( this->m_pSkillStatsDataTable != nullptr )
	{
		FString tName{ "Default" };
		FSkillDataBasic* pData{ this->m_pSkillStatsDataTable->FindRow< FSkillDataBasic >( FName( *tName ), TEXT( "LookUpOperation" ) ) };
		if ( pData != nullptr )
		{
			this->m_ProjectileClass		= pData->m_ProjectileClass;
			this->m_fSpeed				= pData->m_fSpeed;
			this->m_fHalfOffset			= pData->m_fHalfOffset;
			this->m_fHomingDistance		= pData->m_fHomingDistance;
			this->m_iMovementReduction	= pData->m_iMovementReduction;

			this->m_pModifier = NewObject< UAttributeModifier >( this, "Modifier" );
			if ( this->m_pModifier != nullptr )
			{
				this->m_pModifier->Initialise( "Movement Speed", -this->m_iMovementReduction );
			}
		}
	}
}

bool USkillBasicAttack::ReceiveActivate()
{
	bool bReturn{ false };

	if ( this->m_pOwningController != nullptr )
	{
		this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_BASIC_ATTACK );
		this->m_pOwningController->GetCharacteristics()->AttachModifier( this->m_pModifier );
		this->m_bFirstAttack = false;
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBasicAttack::ReceiveDeactivate()
{
	bool bReturn{ false };

	if ( this->m_pOwningController != nullptr && this->m_bFirstAttack )
	{
		if ( this->m_pOwningController->IsAnimState( EAnimStatePlayer::ASP_BASIC_ATTACK ) )
		{
			this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_IDLE_WALK_RUN );
		}

		this->m_pOwningController->GetCharacteristics()->DetachModifier( this->m_pModifier );
		this->m_bFirstAttack = false;
		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBasicAttack::ReceiveInterupt()
{
	bool bReturn{ false };

	if ( this->m_pOwningController != nullptr && this->m_bFirstAttack )
	{
		if ( this->m_pOwningController->IsAnimState( EAnimStatePlayer::ASP_BASIC_ATTACK ) )
		{
			this->m_pOwningController->SetAnimState( EAnimStatePlayer::ASP_IDLE_WALK_RUN );
		}

		this->m_pOwningController->GetCharacteristics()->DetachModifier( this->m_pModifier );
		this->m_bActivated = false;
		this->m_bFirstAttack = false;


		bReturn = true;
	}

	return ( bReturn );
}

bool USkillBasicAttack::ReceiveTrigger()
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

			vRotator.Yaw += FMath::FRandRange( -m_fHalfOffset, m_fHalfOffset );

			AProjectilePlayer* pTempProjectile = Cast< AProjectilePlayer >( UGameplayStatics::BeginSpawningActorFromClass( pOwningCharacter, this->m_ProjectileClass, FTransform( vRotator, vLocation + ( pOwningCharacter->GetActorForwardVector() * 10.0f ) ) ) );

			if ( pTempProjectile )
			{
				float fLifeTime{ static_cast< float >( this->m_pOwningController->GetCharacteristics()->GetRange() ) / this->m_fSpeed };

				pTempProjectile->SetHomingDistance( this->m_fHomingDistance );

				UGameplayStatics::FinishSpawningActor( pTempProjectile, FTransform( vRotator, vLocation + ( pOwningCharacter->GetActorForwardVector() * 10.0f ) ) );

				pTempProjectile->Initialize( this->m_pOwningController, this->m_fSpeed, this->m_pOwningController->GetCharacteristics()->GetDamage(), fLifeTime );

			}

			if ( this->m_pTriggerParticles != nullptr )
			{
				UGameplayStatics::SpawnEmitterAttached( this->m_pTriggerParticles,
														pMeshComponent,
														FName( "Delilah_rigLPalm_socket" ) );
			}

			this->m_bFirstAttack = true;
			this->m_fTimeRemaining = this->m_fCooldown;
		}
	}

	return ( bReturn );
}