// Copyright 2015 Half Tower, Inc. All Rights Reserved.

// Precompiled Header
#include "Souls.h"

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Attribute/Attribute.h"
#include "../../Player/Controller/ControllerPlayer.h"
#include "../Characters/EnemyCharacter.h"
#include "Bosses/BossControllerBase.h"

// This Includes
#include "EnemyControllerBase.h"

// Implementation

// Console variable
// Aggros all enemies
static TAutoConsoleVariable< int32 > AggroAllEnemies( TEXT( "AggroAllEnemies" ),
													  0,
													  TEXT( "Aggros all enemies to the player" ),
													  ECVF_Scalability | ECVF_RenderThreadSafe );

// Constructor
AEnemyControllerBase::AEnemyControllerBase( const FObjectInitializer& _krtInitialiser ) :
	Super( _krtInitialiser ),
	m_eCurrentState( EEnemyState::ENEMY_STATE_IDLE ),
	m_fCooldown( 0.0f ),
	m_fHitCooldown( 0.0f ),
	m_fFreezeLength( 0.0f ),
	m_bFrozen( false )
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	// this->ToggleTick();
}


// Called every frame
// Decreases cooldown
void AEnemyControllerBase::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	if( this->m_bFrozen )
	{
		this->m_fFreezeLength -= DeltaSeconds;
		
		if( this->m_fFreezeLength <= 0.0f )
		{
			this->UnfreezeEnemy();
		}
	}
	else
	{
		if( this->GetCurrentCooldown() > 0.0f )
		{
			this->SetCurrentCooldown( this->GetCurrentCooldown() - DeltaSeconds );
		}

		if( this->GetCurrentHitCooldown() > 0.0f )
		{
			this->SetCurrentHitCooldown( this->GetCurrentHitCooldown() - DeltaSeconds );
		}

		this->ProcessState();

		if( this->m_pCharacteristics != nullptr )
		{
			this->m_pCharacteristics->Process( DeltaSeconds );
		}
	}
}

void AEnemyControllerBase::BeginPlay()
{
	this->m_pCharacteristics = NewObject< UCharacteristics >( this, "Characteristics" );
	this->m_pCharacteristics->Initialise( this->m_Name );
	this->m_pCharacteristics->GetOnDeathDelegate().AddDynamic( this, &AEnemyControllerBase::Death );
	this->m_pCharacteristics->GetOnTakeDamageDelegate().AddDynamic( this, &AEnemyControllerBase::ReceivedDamage );
	this->m_pCharacteristics->GetOnPoisonBeginDelegate().AddDynamic( this, &AEnemyControllerBase::BeginPoisonParticles );
	this->m_pCharacteristics->GetOnPoisonEndDelegate().AddDynamic( this, &AEnemyControllerBase::EndPoisonParticles );
}

// Accessor Function
// Returns value of m_eCurrentState
EEnemyState AEnemyControllerBase::GetCurrentState() const
{
	return( this->m_eCurrentState );
}

// Accessor Function
// Sets value of m_eCurrentState
void AEnemyControllerBase::SetCurrentState( const EEnemyState& _kreNewState )
{
	Cast< AEnemyCharacter >( this->GetCharacter() )->UnfreezeAnimation();

	this->m_eCurrentState = _kreNewState;
	this->OnStateChange( this->m_eCurrentState );
}

// Accessor Function
// Returns value of m_fCooldown
float AEnemyControllerBase::GetCurrentCooldown() const
{
	return( this->m_fCooldown );
}

// Accessor Function
// Returns value of m_pCharacteristics
UCharacteristics* AEnemyControllerBase::GetCharacteristics()
{
	return ( this->m_pCharacteristics );
}


// Accessor Function
// Sets value of m_fCooldown
void AEnemyControllerBase::SetCurrentCooldown( const float& _krfNewCooldown )
{
	this->m_fCooldown = _krfNewCooldown;
}

// Accessor Function
// Returns value of m_fHitCooldown
float AEnemyControllerBase::GetCurrentHitCooldown() const
{
	return( m_fHitCooldown );
}

// Accessor Function
// Sets value of m_fHitCooldown
void AEnemyControllerBase::SetCurrentHitCooldown( const float& _krfNewHitCooldown )
{
	m_fHitCooldown = _krfNewHitCooldown;
}

// Accessor Function
// Sets value of m_Name
void AEnemyControllerBase::SetName( const FName& _krtName )
{
	m_Name = _krtName;
}

// Virtual Function
// Intended to be overridden
bool AEnemyControllerBase::DetectPlayer()
{
	// TODO - Ethan : Magic Numbers
	if( ( this->GetPawn()->GetDistanceTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ) ) < 850.0f ) &&
		( LineOfSightTo( UGameplayStatics::GetPlayerCharacter( this->GetWorld(), 0 ), this->GetPawn()->GetActorLocation() ) ) )
	{
		AlertOtherEnemies();

		return( true );
	}
	
	return( AggroAllEnemies.GetValueOnGameThread() > 0 );
}

void AEnemyControllerBase::AlertOtherEnemies()
{
	// Find all other enemies in radius
	TArray< FOverlapResult > OverlapList;
	this->GetWorld()->OverlapMultiByObjectType( OverlapList,
												this->GetPawn()->GetActorLocation(),
												FQuat::Identity,
												FCollisionObjectQueryParams( ECC_TO_BITFIELD( ECC_GameTraceChannel3 ) ), // Enemy Collision Channel
												FCollisionShape::MakeSphere( 750.0f ) ); // , // TODO - Ethan : Magic Numbers

	// Set enemy state to seeking
	for( int iIndex{ 0 }; iIndex < OverlapList.Num(); iIndex++ )
	{
		AEnemyCharacter* pEnemy{ Cast< AEnemyCharacter >( OverlapList[ iIndex ].GetActor() ) };

		if( pEnemy != nullptr &&
			!Cast< ABossControllerBase >( pEnemy->GetController() ) )
		{
			Cast< AEnemyControllerBase >( pEnemy->GetController() )->SetCurrentState( EEnemyState::ENEMY_STATE_SEEKING );
		}
	}
}

// Virtual Function
// Intended to be overridden
void AEnemyControllerBase::ProcessState()
{

}

// Virtual Function
// Intended to be overridden
FVector AEnemyControllerBase::GetGoToLocation()
{
	return( FVector( 0.0f, 0.0f, 0.0f ) );
}

// Virtual Function
// Intended to be overridden
void AEnemyControllerBase::Attack()
{

}

// Blueprint Function
// Allows variable length hit animation
void AEnemyControllerBase::EnterHitState( float _fAnimDuration )
{
	if( m_fHitCooldown <= 0.0f &&
		FMath::RandRange( 0, 2 ) == 0 )
	{
		m_fHitCooldown = _fAnimDuration;
		this->SetCurrentState( EEnemyState::ENEMY_STATE_PRE_HIT );
		this->SetCurrentCooldown( 0.0f );
	}
}

// Bound Function
// Binds to OnDeath delegate
void AEnemyControllerBase::Death()
{
	Cast< AEnemyCharacter >( this->GetCharacter() )->StopSeekers();

	this->ReceiveDeath();

	this->GetWorld()->DestroyActor( this->GetPawn() );
	this->Destroy();
	// TODO:	Handle death 
}

void AEnemyControllerBase::ReceivedDamage( int32 _iAmount )
{
	auto pCharacter = Cast< AEnemyCharacter >( this->GetCharacter() );
	if ( pCharacter != nullptr )
	{
		pCharacter->SpawnIndicator( static_cast< float >( _iAmount ) );
	}
}

void AEnemyControllerBase::ReceiveDeath()
{
	// ...
}

void AEnemyControllerBase::ToggleTick()
{
	SetActorTickEnabled( !IsActorTickEnabled() );
	Cast<AEnemyCharacter>( this->GetPawn() )->ToggleAllTicks();
}

void AEnemyControllerBase::SetTick( bool _NewTick )
{
	SetActorTickEnabled( _NewTick );
	Cast<AEnemyCharacter>( this->GetPawn() )->SetAllTicks( _NewTick );
}

void AEnemyControllerBase::BeginPoisonParticles()
{
	// Check if there is an available character
		// True - begin the poison particles
	auto pCharacter = Cast< AEnemyCharacter >( this->GetCharacter() );
	if ( pCharacter != nullptr )
	{
		pCharacter->BeginPoisonParticles();
	}
}

void AEnemyControllerBase::EndPoisonParticles()
{
	// Check if there is an available character
		// True - end the poison particles
	auto pCharacter = Cast< AEnemyCharacter >( this->GetCharacter() );
	if ( pCharacter != nullptr )
	{
		pCharacter->EndPoisonParticles();
	}
}

void AEnemyControllerBase::FreezeEnemy( float _fFreezeLength )
{
	this->m_fFreezeLength = _fFreezeLength;
	this->m_bFrozen = true;

	this->GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
	Cast< AEnemyCharacter >( this->GetCharacter() )->FreezeCharacter();
}

void AEnemyControllerBase::UnfreezeEnemy()
{
	this->m_bFrozen = false;

	Cast< AEnemyCharacter >( this->GetCharacter() )->UnfreezeCharacter();
}