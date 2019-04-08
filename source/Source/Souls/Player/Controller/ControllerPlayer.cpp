// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Ensure STL min/max used
#define NOMINMAX

// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include <algorithm>

// Library Includes

// Local Includes
#include "../../Attribute/Attribute.h"
#include "../../Attribute/Characteristics.h"
#include "../../HUD/WidgetBase.h"
#include "../../Inventory/Inventory.h"
#include "../../Enemy/Characters/EnemyCharacter.h"
#include "../../Skill/Active/SkillActive.h"
#include "../../Skill/Active/SkillBasicAttack.h"
#include "../../Skill/Effect/SkillEffectPoison.h"
#include "../../Skill/Interactable/SkillDrop.h"
#include "../../Skill/Interactable/SkillInteractable.h"
#include "../../Skill/SkillBase.h"
#include "../../Skill/SkillManager.h" 
#include "../../SoulsCheatManager.h"
#include "../../SoulsGameInstance.h"
#include "../../SoulsGameMode.h"
#include "../Animation/AnimInstancePlayer.h"
#include "../Character/CharacterPlayer.h"

//This Include
#include "ControllerPlayer.h"

// Implementation
AControllerPlayer::AControllerPlayer( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser ),
	m_vFacingVector( 0.0f, 0.0f, 0.0f ),
	m_bAttacking( false ),
	m_fPreWinDuration( 0.2f ),
	m_fPreWinTimeDilation( 0.1f )
{
	// Set mouse state
	this->bShowMouseCursor = true;
	this->DefaultMouseCursor = EMouseCursor::Default;

	// Get the class for the widget
	static ConstructorHelpers::FClassFinder< UWidgetBase > PlayerWidget( TEXT( "/Game/HUD/BP_HUD" ) );
	this->m_pWidgetClass = PlayerWidget.Class;
		
	// Set cheat class
	this->CheatClass = USoulsCheatManager::StaticClass();
}

UCharacteristics* AControllerPlayer::GetCharacteristics()
{
	return ( this->m_pCharacteristics );
}

UInventory* AControllerPlayer::GetInventory()
{
	return ( this->m_pInventory );
}

UWidgetBase* AControllerPlayer::GetCurrentWidget()
{
	return( this->m_pWidgetInstance );
}

UAnimInstancePlayer* AControllerPlayer::GetAnimInstancePlayer()
{
	return ( this->m_pAnimInstance );
}

void AControllerPlayer::SetAnimState( EAnimStatePlayer _eState )
{
	this->m_pAnimInstance->SetState( _eState );
}

bool AControllerPlayer::IsAnimState( EAnimStatePlayer _eState )
{
	return ( this->m_pAnimInstance->GetState() == _eState );
}

FText AControllerPlayer::GetDebugScreenText()
{
	FVector vPosition{ this->GetCharacter()->GetActorLocation() };
	FString tReturn{ ( "Position < " 
					   " x : " + FString::FromInt( static_cast< int32 >( vPosition.X ) ) + ", "
					   " y : " + FString::FromInt( static_cast< int32 >( vPosition.Y ) ) + ", "
					   " z : " + FString::FromInt( static_cast< int32 >( vPosition.Z ) ) + " >"
					   "\nHealth : " + FString::FromInt( this->m_pCharacteristics->GetHealth() ) +
					   "\nDamage : " + FString::FromInt( this->m_pCharacteristics->GetDamage() ) +
					   "\nAttack Speed : " + FString::FromInt( this->m_pCharacteristics->GetAttackSpeed() ) +
					   "\nMovement Speed : " + FString::FromInt( this->m_pCharacteristics->GetMovementSpeed() ) +
					   "\nInvincibility : " + ( this->m_pCharacteristics->GetInvincible() ? "Enabled" : "Disabled" ) +
					   "\nWin Condition : " + ( m_bWinScreenDisabled ? "Disabled" : "Enabled" ) ) };

	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		tReturn += "\nConditions Remaining : " + WinConditionToString( pGameMode->GetRemainingConditions() );
	}

	return ( FText::FromString( tReturn ) );
}

bool AControllerPlayer::IsLightningActive() const
{
	return ( this->m_bLightningActive );
}

void AControllerPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the animation instance if available
	if ( this->GetCharacter() != nullptr )
	{
		auto pAnimInstance = Cast< UAnimInstancePlayer >( this->GetCharacter()->GetMesh()->GetAnimInstance() );
		if ( pAnimInstance != nullptr )
		{
			this->m_pAnimInstance = pAnimInstance;
		}
	}

	// Create the characteristics and pass appropriate data information
	this->m_pCharacteristics = NewObject< UCharacteristics >( this, TEXT( "Characteristics" ) );
	this->m_pCharacteristics->Initialise( TEXT( "Player" ) );
	this->m_pCharacteristics->GetOnDeathDelegate().AddDynamic( this, &AControllerPlayer::ReceiveDeath );

	// Create the inventory
	this->m_pInventory = NewObject< UInventory >( this, TEXT( "Inventory" ) );
	this->m_pInventory->Initialise( 4, 8, this, this->m_pCharacteristics, this->m_pAnimInstance ); // TODO - Aiden:	Data Drive
	this->m_pInventory->GetOnDropSkillDelegate().AddDynamic( this, &AControllerPlayer::DropSkillEffect );

	// Initialise the HUD widget for game
	if ( this->m_pWidgetClass != nullptr )
	{
		this->m_pWidgetInstance = CreateWidget< UWidgetBase >( this, this->m_pWidgetClass );

		if ( this->m_pWidgetInstance != nullptr )
		{
			this->m_pWidgetInstance->SetVisibility( ESlateVisibility::Visible );
			this->m_pWidgetInstance->AddToViewport();
			this->m_pWidgetInstance->Initialise( this );
			this->m_pInventory->AttachWidgetIcons( this->m_pWidgetInstance->m_arrInventoryIcons );
			this->m_pWidgetInstance->GetOnStateChangeDelegate().AddDynamic( this, &AControllerPlayer::WidgetStateChange );
		}
	}	

	// Perform appropriate setup of widget based on current state of game
	USoulsGameInstance* pInstance{ Cast< USoulsGameInstance >( this->GetGameInstance() ) };
	if ( pInstance != nullptr )
	{
		pInstance->GetSkillManager()->Initialise();

		if ( pInstance->GetIsStartUp() )
		{
			this->m_pWidgetInstance->SetState( EWidgetState::WS_MAINMENU );
			this->SetPause( true );
			pInstance->SetIsStartUp( false );
		}
	}

	m_Destination = this->GetPawn()->GetActorLocation();

	// Create basic attack for the player
	this->m_pBasicAttack = NewObject< USkillBasicAttack >( this, TEXT( "BasicAttack" ) );
	this->m_pBasicAttack->Initialise();
	this->m_pBasicAttack->SetOwningController( this );

	// Initialise the characteristics
	this->m_pCharacteristics->GetMovementSpeedAttribute()->GetOnModifiedDelegate().AddDynamic( this, &AControllerPlayer::MovementAttributeModified );
	this->m_pCharacteristics->GetAttackSpeedAttribute()->GetOnModifiedDelegate().AddDynamic( this, &AControllerPlayer::AttackSpeedAttributeModified );
	this->m_pCharacteristics->GetHealthAttribute()->GetOnAlterFailDelegate().AddDynamic( this, &AControllerPlayer::HealthAlterFailed );
	this->MovementAttributeModified( this->m_pCharacteristics->GetMovementSpeed() );	
	this->AttackSpeedAttributeModified( this->m_pCharacteristics->GetAttackSpeed() );
	this->m_pCharacteristics->GetOnHealthChangeDelegate().AddDynamic( this->m_pWidgetInstance, &UWidgetBase::SetHealthDesired );
	this->m_pCharacteristics->GetOnHealthSetDelegate().AddDynamic( this->m_pWidgetInstance, &UWidgetBase::SetHealth );

	auto pCharacter = Cast< ACharacterPlayer >( this->GetCharacter() );
	if ( pCharacter != nullptr )
	{
		this->m_pCharacteristics->GetHealthAttribute()->GetOnRegenBeginDelegate().AddDynamic( pCharacter, &ACharacterPlayer::BeginRegenParticles );
		this->m_pCharacteristics->GetHealthAttribute()->GetOnRegenEndDelegate().AddDynamic( pCharacter, &ACharacterPlayer::EndRegenParticles );
	}

	this->SetInputMethod( this->m_eInputMethod );

	// Setup win condition for game
	auto pGameMode = Cast< ASoulsGameMode >( this->GetWorld()->GetAuthGameMode() );
	if ( pGameMode != nullptr )
	{
		// TODO - Aiden:	Data drive this
		pGameMode->StartGame( EWinCondition::WC_GREAT_KNIGHT | EWinCondition::WC_CATHEDERAL_GUARDIAN );
		pGameMode->SetWinConditionDisabled( this->m_bWinScreenDisabled );
		pGameMode->GetOnGameWonDelegate().AddDynamic( this, &AControllerPlayer::ReceiveWin );
	}

	this->m_bAttacking = false;

	this->m_pWidgetInstance->SetHealthDesired( 1.0f );
	this->GetCharacter()->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AControllerPlayer::PlayerTick( float _fDeltaTime )
{
	Super::PlayerTick( _fDeltaTime );

	if ( this->m_bPreWinActive )
	{
		this->m_fPreWinRemaining -= _fDeltaTime;
		if ( this->m_fPreWinRemaining <= 0.0f )
		{
			this->m_bPreWinActive = false;
			UGameplayStatics::SetGlobalTimeDilation( this->GetWorld(), 1.0f );
			this->m_pWidgetInstance->ChangeState( EWidgetState::WS_WINMENU );
		}
	}

	if ( this->m_eGameInputState == EGameInputState::GIS_DEFAULT && 
		 this->GetCharacter() != nullptr )
	{
		if ( this->m_vFacingVector.IsNearlyZero() && this->m_bAttacking )
		{
			this->m_bAttacking = !this->m_pBasicAttack->Deactivate();
		}
		else if ( !this->m_vFacingVector.IsNearlyZero() && this->m_pActiveSkill == nullptr )
		{
			this->m_pBasicAttack->Activate();
		}


		// If a button is being pressed
		if ( !this->m_vFacingVector.IsNearlyZero() )
		{
			// Get controlled pawn
			ACharacterPlayer* pPlayer{ Cast< ACharacterPlayer >( this->GetPawn() ) };
			if ( pPlayer != nullptr )
			{
				// Calculate lookat rotation
				FRotator tLookAtRotation{ 0.0f, this->m_vFacingVector.Rotation().Yaw, 0.0f };

				// Set Rotation
				pPlayer->SetActorRotation( tLookAtRotation );
			}

		}
	}

	if ( this->m_bAttacking )
	{
		this->m_pBasicAttack->Trigger();
	}

	if ( this->m_bPoisonTrailActive )
	{
		this->m_fPoisonTrailDuration -= _fDeltaTime;
		if ( this->m_fPoisonTrailDuration > 0.0f )
		{
			this->m_fPoisonTrailTimeElapsed += _fDeltaTime;
			if ( this->m_fPoisonTrailTimeElapsed > this->m_fPoisonTrailSpawnRate )
			{
				auto pMesh = this->GetCharacter()->GetMesh();
				FVector vLocation{ pMesh->GetSocketByName( FName( "ground_socket" ) )->GetSocketLocation( pMesh ) };
				FRotator vRotator{ this->GetCharacter()->GetActorRotation() };

				auto pPoison = Cast< ASkillEffectPoison >( UGameplayStatics::BeginSpawningActorFromClass(
					this->GetWorld(),
					this->m_pPoisonTrailSpawn,
					FTransform( vRotator, vLocation ) ) );

				if ( pPoison != nullptr )
				{
					pPoison->Initialise( this->m_fPoisonEffectLifetime,
										 this->m_fPoisonEffectRadius,
										 this->m_fPoisonDuration,
										 this->m_fPoisonRate,
										 this->m_iPoisonAmount );

					UGameplayStatics::FinishSpawningActor( pPoison, FTransform( vRotator, vLocation ) );
				}

				this->m_fPoisonTrailTimeElapsed -= this->m_fPoisonTrailSpawnRate;
			}
		}
		else
		{
			this->EndPoisonTrail();
		}
	}
	else if ( this->m_eGameInputState != EGameInputState::GIS_DEFAULT && this->m_bAttacking )
	{
		this->m_bAttacking = !this->m_pBasicAttack->Deactivate();
	}

	// Check if lightning has been activated
		// True - find next target and move to them
	if ( this->m_bLightningActive )
	{
		// Check if delay has been met
			// True - attempt to find new target 
		this->m_fLightningTimeElapsed += _fDeltaTime;
		if ( this->m_fLightningTimeElapsed > this->m_fLightningArcDelay )
		{
			this->m_fLightningTimeElapsed -= this->m_fLightningArcDelay;

			// Check if there is a current lightning target
				// True - find valid target and chain to it
				// False - signal end of the lightning
			if ( this->m_pLightningTargetCurr == nullptr )
			{
				this->EndLightning();
			}
			else
			{
				// Store out currents information
				FVector vPositionCurr{ this->m_pLightningTargetCurr->GetActorLocation() };

				// Check if there are arc particles to spawn
					// True - spawn particles between previous target and current
				if ( this->m_pLightningArcParticles != nullptr )
				{
					// Determine location of previous point for arc
					FVector vPositionPrev{ this->m_pLightningTargetPrev != nullptr ?
						this->m_pLightningTargetPrev->GetActorLocation() :
						this->GetCharacter()->GetActorLocation() };

					// Spawn particles and update their source and target location
					auto pArcParticles = UGameplayStatics::SpawnEmitterAtLocation( this->GetWorld(),
																				   this->m_pLightningArcParticles,
																				   vPositionPrev,
																				   FRotator::ZeroRotator,
																				   false );
					pArcParticles->SetBeamSourcePoint( 0, vPositionPrev, 0 );
					pArcParticles->SetBeamTargetPoint( 0, vPositionCurr, 0 );

					if ( this->m_pLightningSoundCue != nullptr )
					{
						UGameplayStatics::PlaySoundAtLocation( this->GetWorld(), this->m_pLightningSoundCue, this->GetCharacter()->GetActorLocation() );
					}

					// Add particles to queue and signal for cleanup
					this->m_queueLightningParticles.Enqueue( pArcParticles );
					this->m_bLightningCleanup = true;
				}


				// Add current targets to target pool
				this->m_arrLightningTargets.Add( this->m_pLightningTargetCurr );

				// Apply damage to the current target and update with falloff 
				this->LightningTarget( this->m_pLightningTargetCurr, static_cast< float >( this->m_iLightningDamageAmount ) );
				this->m_iLightningDamageAmount = static_cast< int32 >( static_cast< float >( this->m_iLightningDamageAmount ) * this->m_fLightningDamageFallOff );
				this->m_iLightningMaxChain--;

				// Find closest target 
				float fClosestDistance{ MAX_FLT };
				AEnemyCharacter* pClosestTarget{ nullptr };

				// Check if possible to chain to new target
					// True - locate closest target not already targeted
				if ( this->m_iLightningMaxChain > 0 && this->m_iLightningDamageAmount > 0 )
				{
					// Perform a sphere collision to detect enemies
					TArray< FOverlapResult > arrOverlapList;
					this->GetWorld()->OverlapMultiByObjectType( arrOverlapList,
																vPositionCurr,
																FQuat::Identity, 
																FCollisionObjectQueryParams::AllDynamicObjects,//ECC_TO_BITFIELD( ECC_GameTraceChannel3 ) | ECC_TO_BITFIELD( ECC_GameTraceChannel6 ), // Enemy Trace Channel
																FCollisionShape::MakeSphere( this->m_fLightningArcDistance ),
																FCollisionQueryParams{ TEXT( "LightningOverlap" ), false, this->m_pLightningTargetCurr } );

					for ( auto& rOverlap : arrOverlapList )
					{
						// Check if overlapped actor is an enemy character
							// True - Determine if it is able to be hit
						auto pTarget = Cast< AEnemyCharacter >( rOverlap.GetActor() );
						if ( pTarget != nullptr )
						{
							// Check if target is closer than current and not already a target
								// True - update closest target
							FVector vTargetDirection{ pTarget->GetActorLocation() - vPositionCurr };
							float fDistance{ vTargetDirection.Size() };
							if ( fDistance < fClosestDistance )
							{
								// Find index of target in list
								int iTarget{ 0 }, iMaxTargets{ this->m_arrLightningTargets.Num() };
								for ( ; iTarget < iMaxTargets && this->m_arrLightningTargets[ iTarget ] != pTarget; iTarget++ );

								// Check that target isn't already target
									// True - set as closest target
								if ( iTarget == iMaxTargets )
								{
									fClosestDistance = fDistance;
									pClosestTarget = pTarget;
								}
							}
						}
					}
				}

				// Update target storage
				this->m_pLightningTargetPrev = this->m_pLightningTargetCurr;
				this->m_pLightningTargetCurr = pClosestTarget;

			}
		}
	}

	// Check if lightning particles need to be cleaned up
		// True - attempt to clean up new particles
	if ( this->m_bLightningCleanup )
	{
		// Check if delay has been met
			// True - clean up particles
		this->m_fLightningCleanupTimeElapsed += _fDeltaTime;
		if ( this->m_fLightningCleanupTimeElapsed > this->m_fLightningCleanupDelay )
		{
			this->m_fLightningCleanupTimeElapsed -= this->m_fLightningCleanupDelay;

			// Check if there are particles to clean
				// True - clean up particles
				// False - signal end of cleanup
			UParticleSystemComponent* pParticles{ nullptr };
			if ( this->m_queueLightningParticles.Dequeue( pParticles ) )
			{
				pParticles->DeactivateSystem();
				pParticles->SetHiddenInGame( true, true );
			}
			else
			{
				this->m_bLightningCleanup = false;
			}
		}
	}
	
	// Continue to update the player attack
	if ( m_pBasicAttack != nullptr )
	{
		this->m_pBasicAttack->Process( _fDeltaTime );
	}

	if ( this->m_pCharacteristics != nullptr )
	{
		this->m_pCharacteristics->Process( _fDeltaTime );
	}

	if ( this->m_pInventory != nullptr )
	{
		this->m_pInventory->Process( _fDeltaTime );
	}

	if ( this->m_pWidgetInstance != nullptr )
	{
		this->m_pWidgetInstance->Process( _fDeltaTime );
	}
}

void AControllerPlayer::Teleport( FVector _vPosition, bool _bSnapTerrain )
{
	// TODO - Aiden:	Re-write this function, seriously where is your head atm?
		// Get the current players position
	FVector tPositionCurrent( this->GetCharacter()->GetActorLocation() );
	FVector tPositionTeleport( _vPosition );

	// Line trace in the positive Z
	FHitResult tUp;
	this->GetWorld()->LineTraceSingleByObjectType(
		tUp,
		tPositionTeleport,
		tPositionTeleport + FVector{ 0.0f, 0.0f, 10000.0f },
		FCollisionObjectQueryParams( ECC_TO_BITFIELD( ECC_WorldStatic ) ) ); // TODO - Aiden:	Change this so that it is only terrain hit

	// Line trace in the negative Z
	FHitResult tDown;
	this->GetWorld()->LineTraceSingleByObjectType(
		tDown,
		tPositionTeleport,
		tPositionTeleport - FVector{ 0.0f, 0.0f, 10000.0f },
		FCollisionObjectQueryParams( ECC_TO_BITFIELD( ECC_WorldStatic ) ) ); // TODO - Aiden:	Change this so that it is only terrain hit

	// Determine if the new position is above a terrain
	if ( tUp.IsValidBlockingHit() && tDown.IsValidBlockingHit() )
	{
		float fDistanceUp{ FVector::Dist( tUp.TraceStart, tUp.TraceEnd ) };
		float fDistanceDown{ FVector::Dist( tDown.TraceStart, tDown.TraceEnd ) };
		tPositionTeleport.Z = fDistanceUp < fDistanceDown ? tUp.Location.Z : tDown.Location.Z;
	}
	else if ( tUp.IsValidBlockingHit() )
	{
		tPositionTeleport.Z = tUp.Location.Z;
	}
	else if ( tDown.IsValidBlockingHit() )
	{
		tPositionTeleport.Z = tDown.Location.Z;
	}
	else if( !_bSnapTerrain && !tDown.IsValidBlockingHit() )
	{
		tPositionTeleport = tPositionCurrent;
	}

	if ( !_bSnapTerrain )
	{
		tPositionTeleport.Z = tPositionCurrent.Z;
	}
	else if ( tPositionTeleport != tPositionCurrent )
	{ 
		tPositionTeleport.Z += this->GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}

	this->GetCharacter()->SetActorLocation( tPositionTeleport );
	this->m_Destination = this->GetCharacter()->GetActorLocation();
}

void AControllerPlayer::TeleportRelative( FVector _vPosition, bool _bSnapTerrain )
{
	this->Teleport( this->GetCharacter()->GetActorLocation() + _vPosition, _bSnapTerrain );
}

void AControllerPlayer::BeginPoisonTrail( float _fTrailDuration,
										  float _fTrailSpawnRate,
										  TSubclassOf< class ASkillEffectPoison > _pTrailSpawn,
										  float _fEffectLifetime,
										  float _fEffectRadius,
										  float _fPoisonDuration,
										  float _fPoisonRate,
										  int32 _iPoisonAmount )
{
	if ( !this->m_bPoisonTrailActive )
	{
		this->m_fPoisonTrailTimeElapsed = 0.0f;
	}

	this->m_bPoisonTrailActive = true;
	this->m_fPoisonTrailDuration = _fTrailDuration;
	this->m_fPoisonTrailSpawnRate = _fTrailSpawnRate;
	this->m_pPoisonTrailSpawn = _pTrailSpawn;
	this->m_fPoisonEffectLifetime = _fEffectLifetime;
	this->m_fPoisonEffectRadius = _fEffectRadius;
	this->m_fPoisonDuration = _fPoisonDuration;
	this->m_fPoisonRate = _fPoisonRate;
	this->m_iPoisonAmount = _iPoisonAmount;
}

void AControllerPlayer::EndPoisonTrail()
{
	this->m_bPoisonTrailActive = false;
	this->m_fPoisonTrailDuration = 0.0f;
	this->m_fPoisonTrailTimeElapsed = 0.0f;
}

bool AControllerPlayer::BeginLightning( AEnemyCharacter* pInitialTarget,
										float _fDamageMultiplier,
										float _fDamageFallOff,
										float _fArcDelay,
										float _fArcDistance,
										int32 _iMaxChain,
										UParticleSystem* _pArcParticles,
										UParticleSystem* _pHitParticles,
										float _fCleanupDelay )
{
	bool bReturn{ false };

	// Check if lightning isn't already in use
		// True - activate the lightning
	if ( !this->m_bLightningActive )
	{
		// Set local variables from passed parameters
		this->m_bLightningActive = true;
		this->m_iLightningDamageAmount = static_cast< int32 >( static_cast< float >( this->m_pCharacteristics->GetDamage() ) * _fDamageMultiplier );
		this->m_fLightningDamageFallOff = _fDamageFallOff;
		this->m_fLightningArcDelay = _fArcDelay;
		this->m_fLightningTimeElapsed = 0.0f;
		this->m_fLightningArcDistance = _fArcDistance;
		this->m_iLightningMaxChain = _iMaxChain;
		this->m_pLightningArcParticles = _pArcParticles;
		this->m_pLightningHitParticles = _pHitParticles;
		this->m_fLightningCleanupDelay = _fCleanupDelay;
		this->m_fLightningCleanupTimeElapsed = 0.0f;
		
		// Set up for the initial round of lightning
		this->m_pLightningTargetCurr = pInitialTarget;
		this->m_pLightningTargetPrev = nullptr;

		bReturn = true;
	}

	return ( bReturn );
}

void AControllerPlayer::EndLightning()
{
	this->m_bLightningActive = false;
	this->m_fLightningTimeElapsed = 0.0f;
	this->m_pLightningTargetCurr = nullptr;
	this->m_pLightningTargetPrev = nullptr;
	this->m_arrLightningTargets.Empty();
}

void AControllerPlayer::LightningTarget( class AEnemyCharacter* pTarget, float _fDamageAmount )
{
	// Check if passed target is valid
		// True - Give it damage and spawn particles at its location
	if ( pTarget != nullptr )
	{
		UGameplayStatics::ApplyDamage( pTarget, _fDamageAmount, this, this->GetCharacter(), UDamageType::StaticClass() );
		if ( this->m_pLightningHitParticles != nullptr )
		{
			UGameplayStatics::SpawnEmitterAtLocation( this->GetWorld(),
													  this->m_pLightningHitParticles,
													  pTarget->GetActorLocation(),
													  pTarget->GetActorRotation() );
		}
	}
}

void AControllerPlayer::BeginCutScene()
{
	this->m_pWidgetInstance->ChangeState( EWidgetState::WS_CUTSCENE );
	this->m_eGameInputState = EGameInputState::GIS_CUTSCENE;
	this->PlayerInvincibilityActivate();
}

void AControllerPlayer::EndCutScene()
{
	this->m_pWidgetInstance->ChangeState( EWidgetState::WS_INGAME );
	this->m_eGameInputState = this->m_pWidgetInstance->GetShowInventory() ? EGameInputState::GIS_INVENTORY : EGameInputState::GIS_DEFAULT;
	this->PlayerInvincibilityDeactivate();

	this->m_Destination = this->GetCharacter()->GetActorLocation();
}

void AControllerPlayer::BasicAttackEnter()
{
	this->m_bAttacking = true;
}

void AControllerPlayer::BasicAttackLeave()
{
	this->m_bAttacking = false;
}

bool AControllerPlayer::ActivateSkill( class USkillActive* _pSkill )
{
	bool bReturn{ false };

	// Check if there isn't a currently active skill and passed skill is valid
		// True - attempt to start passed skill
	if ( this->m_pActiveSkill == nullptr && _pSkill != nullptr )
	{
		if ( this->m_pBasicAttack->Interupt() || !this->m_pBasicAttack->IsActivated() )
		{
			this->m_pActiveSkill = _pSkill;

			// Check if the skill has a cast stun
			// True - update the input method 
			if ( this->m_pActiveSkill->HasCastStun() )
			{
				this->m_eGameInputState = EGameInputState::GIS_CASTSTUN;
				this->m_Destination = this->GetCharacter()->GetActorLocation();
			}

			bReturn = true;
		}
	}

	return ( bReturn );
}

bool AControllerPlayer::DeactivateSkill()
{
	bool bReturn{ false };

	// Check if there is an active skill to deactivate and if it can be stopped
		// True - stop current skill
	if ( this->m_pActiveSkill != nullptr && this->m_pActiveSkill->Deactivate() )
	{
		this->m_pActiveSkill = nullptr;
		this->m_eGameInputState = this->m_pWidgetInstance->GetShowInventory() ? EGameInputState::GIS_INVENTORY : EGameInputState::GIS_DEFAULT;
		bReturn = true;
	}

	return ( bReturn );
}

bool AControllerPlayer::TriggerSkill()
{
	// Check if there is an active skill to trigger and if it can be triggered
	return ( this->m_pActiveSkill != nullptr && this->m_pActiveSkill->Trigger() );
}

bool AControllerPlayer::InteruptSkill()
{
	bool bReturn{ false };

	// Check if there is an active skill to interupt anf if it can be interupted
		// True - stop current skill
	if ( this->m_pActiveSkill != nullptr && this->m_pActiveSkill->Interupt() )
	{
		this->m_pActiveSkill = nullptr;
		this->m_eGameInputState = EGameInputState::GIS_DEFAULT;
		bReturn = true;
	}

	return( bReturn );
}

void AControllerPlayer::ReceiveWin()
{
	if ( !this->m_bPreWinActive )
	{
		this->m_bPreWinActive = true;
		this->m_fPreWinRemaining = this->m_fPreWinDuration;
		UGameplayStatics::SetGlobalTimeDilation( this->GetWorld(), this->m_fPreWinTimeDilation );

		this->GetCharacteristics()->ActivateInvincibility();
	}
}

void AControllerPlayer::ReceiveDeath()
{
	this->PlayerInventoryDropAll();
	this->m_pWidgetInstance->ChangeState( EWidgetState::WS_DEATHMENU );
}

void AControllerPlayer::DropSkillEffect( class USkillBase* _pSkill )
{
	if ( _pSkill != nullptr )
	{
		// TODO - Aiden:	Add drop effect / explosion ( ? )
		auto rLocation = this->GetCharacter()->GetActorLocation();
		auto rRotation = this->GetCharacter()->GetActorRotation();

		auto pSkillDrop = this->GetWorld()->SpawnActor( _pSkill->GetSkillDrop(), &rLocation, &rRotation );
		ASkillDrop* pDrop{ Cast< ASkillDrop >( pSkillDrop ) };
		if ( pDrop != nullptr )
		{
			pDrop->Initialise( _pSkill );
		}
	}
}

void AControllerPlayer::MovementAttributeModified( int32 _iAmount )
{
	this->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = _iAmount;
}

void AControllerPlayer::AttackSpeedAttributeModified( int32 _iAmount )
{
	this->m_pBasicAttack->SetCooldown( 10.0f / static_cast< float >( _iAmount ) );
}

void AControllerPlayer::HealthAlterFailed()
{
	// TODO - Aiden:	Write handling for alter failing.
	GEngine->AddOnScreenDebugMessage( -1, 0.1f, FColor::Cyan, FString{ "That will kill you!" } );
}