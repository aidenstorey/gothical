// Precompiled Header
#include "Souls.h"

// UE4 Includes
#include "ConsoleManager.h"
#include "AudioDevice.h"

// Library Includes

// Local Includes

// This Include
#include "QuarrieDamageBPLibrary.h"

// Implementation

// Console variable
// Allows for debug drawing
static TAutoConsoleVariable< int32 > bDamageDebugDrawing( TEXT( "DamageDebugDrawing" ),
														 0,
														 TEXT( "Determines whether to display debug damage areas" ),
														 ECVF_Scalability | ECVF_RenderThreadSafe );


// Helper Function
// Returns true if component can be damaged
// with a trace from Origin
static bool ComponentIsDamageableFrom( UPrimitiveComponent* VictimComp, FVector const& Origin, AActor const* IgnoredActor, const TArray<AActor*>& IgnoreActors, ECollisionChannel TraceChannel, FHitResult& OutHitResult )
{
	if( VictimComp->ComponentHasTag( "IgnoreDamage" ) )
	{
		return false;
	}

	static FName NAME_ComponentIsVisibleFrom = FName( TEXT( "ComponentIsVisibleFrom" ) );
	FCollisionQueryParams LineParams( NAME_ComponentIsVisibleFrom, true, IgnoredActor );
	LineParams.AddIgnoredActors( IgnoreActors );

	// Do a trace from origin to middle of box
	UWorld* const World = VictimComp->GetWorld();
	check( World );

	FVector const TraceEnd = VictimComp->Bounds.Origin;
	FVector TraceStart = Origin;
	if( Origin == TraceEnd )
	{
		// tiny nudge so LineTraceSingle doesn't early out with no hits
		TraceStart.Z += 0.01f;
	}
	bool const bHadBlockingHit = World->LineTraceSingleByChannel( OutHitResult, TraceStart, TraceEnd, TraceChannel, LineParams );
	//::DrawDebugLine(World, TraceStart, TraceEnd, FLinearColor::Red, true);

	// If there was a blocking hit, it will be the last one
	if( bHadBlockingHit )
	{
		if( OutHitResult.Component == VictimComp )
		{
			// if blocking hit was the victim component, it is visible
			return true;
		}
		else
		{
			// if we hit something else blocking, it's not
			UE_LOG( LogDamage, Log, TEXT( "Radial Damage to %s blocked by %s (%s)" ), *GetNameSafe( VictimComp ), *GetNameSafe( OutHitResult.GetActor() ), *GetNameSafe( OutHitResult.Component.Get() ) );
			return false;
		}
	}

	// didn't hit anything, assume nothing blocking the damage and victim is consequently visible
	// but since we don't have a hit result to pass back, construct a simple one, modeling the damage as having hit a point at the component's center.
	FVector const FakeHitLoc = VictimComp->GetComponentLocation();
	FVector const FakeHitNorm = ( Origin - FakeHitLoc ).GetSafeNormal();		// normal points back toward the epicenter
	OutHitResult = FHitResult( VictimComp->GetOwner(), VictimComp, FakeHitLoc, FakeHitNorm );
	return true;
}

// Constructor
// Purposefully left blank
UQuarrieDamageBPLibrary::UQuarrieDamageBPLibrary( const class FObjectInitializer& PCIP )
	: Super( PCIP )
{

}

// Wrapper for UE4 Radial Damage
// Allows debug drawing of damage area
bool UQuarrieDamageBPLibrary::QApplyRadialDamage( UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw, AActor* DamageCauser, AController* InstigatedByController, bool bDoFullDamage, ECollisionChannel DamagePreventionChannel )
{
	float DamageFalloff = bDoFullDamage ? 0.f : 1.f;
	return QApplyRadialDamageWithFalloff( WorldContextObject, BaseDamage, 0.f, Origin, 0.f, DamageRadius, DamageFalloff, DamageTypeClass, IgnoreActors, bDebugDraw, DamageCauser, InstigatedByController, DamagePreventionChannel );

	// if( bDamageDebugDrawing.GetValueOnGameThread() )
	// {
	// 	DrawDebugSphere( WorldContextObject->GetWorld(), Origin, DamageRadius, 32, FColor( 255, 0, 0 ), false, 0.1f );
	// }
	// return( UGameplayStatics::ApplyRadialDamage( WorldContextObject, BaseDamage, Origin, DamageRadius, DamageTypeClass, IgnoreActors, DamageCauser, InstigatedByController, bDoFullDamage, DamagePreventionChannel ) );
}

// Wrapper for UE4 Radial Damage
// Allows debug drawing of damage area
bool UQuarrieDamageBPLibrary::QApplyRadialDamageWithFalloff( UObject* WorldContextObject, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw, AActor* DamageCauser, AController* InstigatedByController, ECollisionChannel DamagePreventionChannel )
{
	if( bDamageDebugDrawing.GetValueOnGameThread() )
	{
		DrawDebugSphere( WorldContextObject->GetWorld(), Origin, DamageOuterRadius, 32, FColor( 255, 0, 0 ), false, 0.1f );
		DrawDebugSphere( WorldContextObject->GetWorld(), Origin, DamageInnerRadius, 32, FColor( 255, 0, 0 ), false, 0.1f );
	}

	static FName NAME_ApplyRadialDamage = FName( TEXT( "ApplyRadialDamage" ) );
	FCollisionQueryParams SphereParams( NAME_ApplyRadialDamage, false, DamageCauser );

	SphereParams.AddIgnoredActors( IgnoreActors );

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject );
	World->OverlapMultiByObjectType( Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams( FCollisionObjectQueryParams::InitType::AllDynamicObjects ), FCollisionShape::MakeSphere( DamageOuterRadius ), SphereParams );

	// collate into per-actor list of hit components
	TMap<AActor*, TArray<FHitResult> > OverlapComponentMap;
	for( int32 Idx = 0; Idx<Overlaps.Num(); ++Idx )
	{
		FOverlapResult const& Overlap = Overlaps[ Idx ];
		AActor* const OverlapActor = Overlap.GetActor();

		if( OverlapActor &&
			OverlapActor->bCanBeDamaged &&
			OverlapActor != DamageCauser &&
			Overlap.Component.IsValid() )
		{
			FHitResult Hit;
			if( DamagePreventionChannel == ECC_MAX || ComponentIsDamageableFrom( Overlap.Component.Get(), Origin, DamageCauser, IgnoreActors, DamagePreventionChannel, Hit ) )
			{
				TArray<FHitResult>& HitList = OverlapComponentMap.FindOrAdd( OverlapActor );
				HitList.Add( Hit );
			}
		}
	}

	// make sure we have a good damage type
	TSubclassOf<UDamageType> const ValidDamageTypeClass = ( DamageTypeClass == NULL ) ? TSubclassOf<UDamageType>( UDamageType::StaticClass() ) : DamageTypeClass;

	bool bAppliedDamage = false;

	// call damage function on each affected actors
	for( TMap<AActor*, TArray<FHitResult> >::TIterator It( OverlapComponentMap ); It; ++It )
	{
		AActor* const Victim = It.Key();
		TArray<FHitResult> const& ComponentHits = It.Value();

		FRadialDamageEvent DmgEvent;
		DmgEvent.DamageTypeClass = ValidDamageTypeClass;
		DmgEvent.ComponentHits = ComponentHits;
		DmgEvent.Origin = Origin;
		DmgEvent.Params = FRadialDamageParams( BaseDamage, MinimumDamage, DamageInnerRadius, DamageOuterRadius, DamageFalloff );

		Victim->TakeDamage( BaseDamage, DmgEvent, InstigatedByController, DamageCauser );

		bAppliedDamage = true;
	}

	return bAppliedDamage;
}

// Applies damage in a cone
bool UQuarrieDamageBPLibrary::QApplyConicDamageWithFalloff( UObject* WorldContextObject, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageAngle, FVector Direction, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw, AActor* DamageCauser, AController* InstigatedByController, ECollisionChannel DamagePreventionChannel )
{
	//Convert to Radians
	DamageAngle = ( DamageAngle * PI ) / 180;

	if( bDamageDebugDrawing.GetValueOnGameThread() )
	{
		FVector EndPoint = FVector( 1.0f, 0.0f, 0.0f );
		EndPoint = EndPoint.RotateAngleAxis( ( DamageAngle * 180 ) / PI, FVector( 0.0f, 1.0f, 0.0f ) );

		for( int iLineNum = 0; iLineNum < 8; ++iLineNum )
		{
			FRotationMatrix DirectRotation = FRotationMatrix( EndPoint.Rotation() );

			DrawDebugLine( WorldContextObject->GetWorld(), Origin + ( DirectRotation.TransformVector( Direction ) * DamageInnerRadius ), Origin + ( DirectRotation.TransformVector( Direction ) * DamageOuterRadius ), FColor( 255, 0, 0 ), false, 0.1f );

			EndPoint = EndPoint.RotateAngleAxis( 45, FVector( 1.0f, 0.0f, 0.0f ) );
		}

		DrawDebugLine( WorldContextObject->GetWorld(), Origin + ( Direction * DamageInnerRadius ), Origin + ( Direction * DamageOuterRadius ), FColor( 255, 0, 0 ), false, 0.1f );
	}

	static FName NAME_ApplyConicalDamage = FName( TEXT( "ApplyConicalDamage" ) );
	FCollisionQueryParams SphereParams( NAME_ApplyConicalDamage, false, DamageCauser );

	SphereParams.AddIgnoredActors( IgnoreActors );

	TArray<FOverlapResult> Overlaps;
	UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject );
	World->OverlapMultiByObjectType( Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams( FCollisionObjectQueryParams::InitType::AllDynamicObjects ), FCollisionShape::MakeSphere( DamageOuterRadius ), SphereParams );

	TMap<AActor*, TArray<FHitResult> > OverlapComponentMap;
	for( int32 Idx = 0; Idx<Overlaps.Num(); ++Idx )
	{
		FOverlapResult const& Overlap = Overlaps[ Idx ];
		AActor* const OverlapActor = Overlap.GetActor();

		if( OverlapActor &&
			OverlapActor->bCanBeDamaged &&
			OverlapActor != DamageCauser &&
			Overlap.Component.IsValid() )
		{
			FHitResult Hit;
			if( DamagePreventionChannel == ECC_MAX || ComponentIsDamageableFrom( Overlap.Component.Get(), Origin, DamageCauser, IgnoreActors, DamagePreventionChannel, Hit ) )
			{
				TArray<FHitResult>& HitList = OverlapComponentMap.FindOrAdd( OverlapActor );
				HitList.Add( Hit );
			}
		}
	}

	TSubclassOf<UDamageType> const ValidDamageTypeClass = ( DamageTypeClass == NULL ) ? TSubclassOf<UDamageType>( UDamageType::StaticClass() ) : DamageTypeClass;

	bool bAppliedDamage = false;

	for( TMap<AActor*, TArray<FHitResult> >::TIterator It( OverlapComponentMap ); It; ++It )
	{
		AActor* const Victim = It.Key();

		FVector VictimDirection = Victim->GetActorLocation() - Origin;

		VictimDirection.Normalize();
		Direction.Normalize();

		if( acos( FVector::DotProduct( Direction, VictimDirection ) ) < DamageAngle )
		{

			TArray<FHitResult> const& ComponentHits = It.Value();
			FRadialDamageEvent DmgEvent;
			DmgEvent.DamageTypeClass = ValidDamageTypeClass;
			DmgEvent.ComponentHits = ComponentHits;
			DmgEvent.Origin = Origin;
			DmgEvent.Params = FRadialDamageParams( BaseDamage, MinimumDamage, DamageInnerRadius, DamageOuterRadius, DamageFalloff );

			Victim->TakeDamage( BaseDamage, DmgEvent, InstigatedByController, DamageCauser );

			bAppliedDamage = true;
		}
	}

	return bAppliedDamage;
}

// Applies damage in a cone
bool UQuarrieDamageBPLibrary::QApplyConicDamage( UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageDistance, float DamageAngle, FVector Direction, TSubclassOf< class UDamageType > DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw, AActor* DamageCauser, AController* InstigatedByController, bool bDoFullDamage, ECollisionChannel DamagePreventionChannel )
{
	float DamageFalloff = bDoFullDamage ? 1.0f : 0.0f;
	return( UQuarrieDamageBPLibrary::QApplyConicDamageWithFalloff( WorldContextObject, BaseDamage, 0.0f, Origin, 0.0f, DamageDistance, DamageAngle, Direction, DamageFalloff, DamageTypeClass, IgnoreActors, bDebugDraw, DamageCauser, InstigatedByController, DamagePreventionChannel ) );
}

bool UQuarrieDamageBPLibrary::RebindAxisKey( const FString& _krsAxisName, const FKey& _krOldKey, const FKeyEvent& _krNewKey, FKey& _rNewKeyOut )
{
	UInputSettings* GameInputSettings = const_cast<UInputSettings*>( GetDefault< UInputSettings >() );

	if( !GameInputSettings )
	{
		return( false );
	}

	TArray< FInputAxisKeyMapping >& AllAxis = GameInputSettings->AxisMappings;

	bool bRebound = false;

	for( FInputAxisKeyMapping& CurrentAxis : AllAxis )
	{
		if( CurrentAxis.AxisName.ToString() == _krsAxisName &&
			CurrentAxis.Key == _krOldKey )
		{
			CurrentAxis.Key = _krNewKey.GetKey();
			_rNewKeyOut = _krNewKey.GetKey();
			bRebound = true;
			break;
		}
	}

	if( bRebound )
	{
		const_cast< UInputSettings* >( GameInputSettings )->SaveKeyMappings();

		for( TObjectIterator< UPlayerInput > Iterator; Iterator; ++Iterator )
		{
			Iterator->ForceRebuildingKeyMaps( true );
		}
	}

	return( bRebound );
}

bool UQuarrieDamageBPLibrary::RebindActionKey( const FString& _krsActionName, const FKey& _krOldKey, const FKeyEvent& _krNewKey, FKey& _rNewKeyOut )
{
	UInputSettings* GameInputSettings = const_cast< UInputSettings* >( GetDefault< UInputSettings >() );

	if( !GameInputSettings )
	{
		return( false );
	}

	TArray< FInputActionKeyMapping >& AllActions = GameInputSettings->ActionMappings;

	bool bRebound = false;

	for( FInputActionKeyMapping& CurrentAction : AllActions )
	{
		if( CurrentAction.ActionName.ToString() == _krsActionName &&
			CurrentAction.Key == _krOldKey )
		{
			CurrentAction.Key = _krNewKey.GetKey();
			_rNewKeyOut = _krNewKey.GetKey();
			bRebound = true;
			break;
		}
	}

	if( bRebound )
	{
		const_cast< UInputSettings* >( GameInputSettings )->SaveKeyMappings();

		for( TObjectIterator< UPlayerInput > Iterator; Iterator; ++Iterator )
		{
			Iterator->ForceRebuildingKeyMaps( true );
		}
	}

	return( bRebound );
}

void UQuarrieDamageBPLibrary::GetAllAxisKeyBindings( TArray<FInputAxisKeyMapping>& _rBindings )
{
	_rBindings.Empty();

	const UInputSettings* GameInputSettings = GetDefault< UInputSettings >();

	if( !GameInputSettings )
	{
		return;
	}

	_rBindings = GameInputSettings->AxisMappings;
}


void UQuarrieDamageBPLibrary::GetAllActionKeyBindings( TArray< FInputActionKeyMapping >& _rBindings )
{
	_rBindings.Empty();

	const UInputSettings* GameInputSettings = GetDefault< UInputSettings >();

	if( !GameInputSettings )
	{
		return;
	}

	_rBindings = GameInputSettings->ActionMappings;
}


void UQuarrieDamageBPLibrary::SplitInputAxisMapping( FInputAxisKeyMapping _Input, FName& _NameOut, FKey& _KeyOut, float& _ScaleOut )
{
	_NameOut = _Input.AxisName;
	_KeyOut = _Input.Key;
	_ScaleOut = _Input.Scale;
}

void UQuarrieDamageBPLibrary::SplitInputActionMapping( FInputActionKeyMapping _Input, FName& _NameOut, FKey& _KeyOut )
{
	_NameOut = _Input.ActionName;
	_KeyOut = _Input.Key;
}

UGameUserSettings* UQuarrieDamageBPLibrary::GetGameUserSettings()
{
	if( GEngine != nullptr )
	{
		return( GEngine->GameUserSettings );
	}
	else
	{
		return( nullptr );
	}
}

bool UQuarrieDamageBPLibrary::GetAllResolutions( TArray<FString>& _rsResolutionOut )
{
	FScreenResolutionArray AllResolutions;

	if( RHIGetAvailableResolutions( AllResolutions, true ) )
	{
		for( const FScreenResolutionRHI& krCurrentResolution : AllResolutions )
		{
			_rsResolutionOut.AddUnique( FString::FromInt( krCurrentResolution.Width ) + "x" + FString::FromInt( krCurrentResolution.Height ) );
		}

		return( true );
	}

	return( false ); // Failed to obtain resolutions
}

bool UQuarrieDamageBPLibrary::SetScreenResolution( const int32 _kiWidth, const int32 _kiHeight )
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	UserSettings->SetScreenResolution( FIntPoint( _kiWidth, _kiHeight ) );
	UserSettings->RequestResolutionChange( _kiWidth, _kiHeight, UserSettings->GetFullscreenMode(), false );

	ApplySettings();

	return( true );
}

bool UQuarrieDamageBPLibrary::ToggleFullScreen()
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	EWindowMode::Type NewMode = ( UserSettings->GetFullscreenMode() == EWindowMode::Fullscreen ? EWindowMode::Windowed : EWindowMode::Fullscreen );

	UserSettings->SetFullscreenMode( NewMode );
	UserSettings->RequestResolutionChange( UserSettings->GetScreenResolution().X, UserSettings->GetScreenResolution().Y, NewMode, false );

	ApplySettings();

	return( true );
}

bool UQuarrieDamageBPLibrary::IsFullscreen( bool& _bReturn )
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	_bReturn = UserSettings->GetFullscreenMode() == EWindowMode::Fullscreen;
	return( true );
}

bool UQuarrieDamageBPLibrary::ApplySettings()
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	UserSettings->ConfirmVideoMode();
	UserSettings->ApplyNonResolutionSettings();
	UserSettings->SaveSettings();

	return( true );
}

bool UQuarrieDamageBPLibrary::GetQualityLevels( int32& _riAntiAliasing,
												int32& _riEffects,
												int32& _riPostProcessing,
												int32& _riResolution,
												int32& _riShadow,
												int32& _riTexture,
												int32& _riViewDistance )
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	_riAntiAliasing		= UserSettings->ScalabilityQuality.AntiAliasingQuality;
	_riEffects			= UserSettings->ScalabilityQuality.EffectsQuality;
	_riPostProcessing	= UserSettings->ScalabilityQuality.PostProcessQuality;
	_riResolution		= ( UserSettings->ScalabilityQuality.ResolutionQuality / 25 ) - 1;
	_riShadow			= UserSettings->ScalabilityQuality.ShadowQuality;
	_riTexture			= UserSettings->ScalabilityQuality.TextureQuality;
	_riViewDistance		= UserSettings->ScalabilityQuality.ViewDistanceQuality;

	return( true );
}

bool UQuarrieDamageBPLibrary::GetSingleQualityLevel( const EGraphicsOption& _kreOption,
													 int32& _riQualityValue )
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	switch( _kreOption )
	{
	case EGraphicsOption::GRAPHICS_ALL_OPTIONS:
		GetAllOptionsValue( _riQualityValue );
		break;

	case EGraphicsOption::GRAPHICS_ANTI_ALIASING:
		_riQualityValue = UserSettings->ScalabilityQuality.AntiAliasingQuality;
		break;

	case EGraphicsOption::GRAPHICS_EFFECTS:
		_riQualityValue = UserSettings->ScalabilityQuality.EffectsQuality;
		break;

	case EGraphicsOption::GRAPHICS_POST_PROCESSING:
		_riQualityValue = UserSettings->ScalabilityQuality.PostProcessQuality;
		break;

	case EGraphicsOption::GRAPHICS_RESOLUTION:
		_riQualityValue = ( UserSettings->ScalabilityQuality.ResolutionQuality / 25 ) - 1;
		break;

	case EGraphicsOption::GRAPHICS_SHADOW:
		_riQualityValue = UserSettings->ScalabilityQuality.ShadowQuality;
		break;

	case EGraphicsOption::GRAPHICS_TEXTURE:
		_riQualityValue = UserSettings->ScalabilityQuality.TextureQuality;
		break;

	case EGraphicsOption::GRAPHICS_VIEW_DISTANCE:
		_riQualityValue = UserSettings->ScalabilityQuality.ViewDistanceQuality;
		break;

	default:
		break;
	}

	return( true );
}

bool UQuarrieDamageBPLibrary::GetAllOptionsValue( int32& _riAllOptionsValue )
{
	int32 OptionsArray[ 7 ];

	GetQualityLevels( OptionsArray[ 0 ], 
					  OptionsArray[ 1 ], 
					  OptionsArray[ 2 ], 
					  OptionsArray[ 3 ], 
					  OptionsArray[ 4 ], 
					  OptionsArray[ 5 ], 
					  OptionsArray[ 6 ] );

	int32 ComparisonValue = OptionsArray[ 0 ];

	for( int32 i = 1; i < 7; ++i )
	{
		if( ComparisonValue != OptionsArray[ i ] )
		{
			_riAllOptionsValue = 4;
			return( true );
		}
	}

	_riAllOptionsValue = ComparisonValue;
	return( true );
}

bool UQuarrieDamageBPLibrary::SetQualityLevels( const int32& _kriAntiAliasing,
												const int32& _kriEffects,
												const int32& _kriPostProcessing,
												const int32& _kriResolution,
												const int32& _kriShadow,
												const int32& _kriTexture,
												const int32& _kriViewDistance )
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	UserSettings->ScalabilityQuality.AntiAliasingQuality = _kriAntiAliasing;
	UserSettings->ScalabilityQuality.EffectsQuality		 = _kriEffects;
	UserSettings->ScalabilityQuality.PostProcessQuality  = _kriPostProcessing;
	UserSettings->ScalabilityQuality.ResolutionQuality   = _kriResolution;
	UserSettings->ScalabilityQuality.ShadowQuality		 = _kriShadow;
	UserSettings->ScalabilityQuality.TextureQuality		 = _kriTexture;
	UserSettings->ScalabilityQuality.ViewDistanceQuality = _kriViewDistance;

	ApplySettings();

	return( true );
}

bool UQuarrieDamageBPLibrary::SetSingleQualityLevel( const EGraphicsOption& _kreOption,
													 const int32& _kriNewValue )
{
	UGameUserSettings* UserSettings = GetGameUserSettings();

	if( UserSettings == nullptr )
	{
		return( false );
	}

	// TODO - Ethan : Make this switch dum
	switch( _kreOption )
	{
	case EGraphicsOption::GRAPHICS_ALL_OPTIONS:
		return( UQuarrieDamageBPLibrary::SetQualityLevels( _kriNewValue,
														   _kriNewValue,
														   _kriNewValue,
														   ( _kriNewValue + 1 ) * 25,
														   _kriNewValue,
														   _kriNewValue,
														   _kriNewValue ) );
		break;

	case EGraphicsOption::GRAPHICS_ANTI_ALIASING:
		UserSettings->ScalabilityQuality.AntiAliasingQuality = _kriNewValue;
		break;

	case EGraphicsOption::GRAPHICS_EFFECTS:
		UserSettings->ScalabilityQuality.EffectsQuality = _kriNewValue;
		break;

	case EGraphicsOption::GRAPHICS_POST_PROCESSING:
		UserSettings->ScalabilityQuality.PostProcessQuality = _kriNewValue;
		break;

	case EGraphicsOption::GRAPHICS_RESOLUTION:
		UserSettings->ScalabilityQuality.ResolutionQuality = ( _kriNewValue + 1 ) * 25;
		break;

	case EGraphicsOption::GRAPHICS_SHADOW:
		UserSettings->ScalabilityQuality.ShadowQuality = _kriNewValue;
		break;

	case EGraphicsOption::GRAPHICS_TEXTURE:
		UserSettings->ScalabilityQuality.TextureQuality = _kriNewValue;
		break;

	case EGraphicsOption::GRAPHICS_VIEW_DISTANCE:
		UserSettings->ScalabilityQuality.ViewDistanceQuality = _kriNewValue;
		break;

	default:
		break;
	}

	ApplySettings();

	return( true );
}

bool UQuarrieDamageBPLibrary::GetVolumeLevel( FString _SoundClassName, float& _rfVolume )
{
	FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();

	if( AudioDevice == nullptr )
	{
		return( false );
	}

	for( TMap< USoundClass*, FSoundClassProperties >::TIterator Iter( AudioDevice->SoundClasses ); Iter; ++Iter )
	{
		USoundClass* CurrentSoundClass = Iter.Key();

		if( CurrentSoundClass &&
			CurrentSoundClass->GetFullName().Find( _SoundClassName ) != INDEX_NONE )
		{
			_rfVolume = CurrentSoundClass->Properties.Volume;
		}
	}

	return( true );
}

bool UQuarrieDamageBPLibrary::SetVolumeLevel( FString _SoundClassName, float _fNewVolume )
{
	FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();

	if( AudioDevice == nullptr )
	{
		return( false );
	}

	for( TMap< USoundClass*, FSoundClassProperties >::TIterator Iter( AudioDevice->SoundClasses ); Iter; ++Iter )
	{
		USoundClass* CurrentSoundClass = Iter.Key();

		if( CurrentSoundClass &&
			CurrentSoundClass->GetFullName().Find( _SoundClassName ) != INDEX_NONE )
		{
			CurrentSoundClass->Properties.Volume = _fNewVolume;
		}

		// GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Yellow, CurrentSoundClass->GetFullName() );
	}

	return( true );
}