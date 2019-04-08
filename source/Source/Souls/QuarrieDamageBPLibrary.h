#pragma once

// UE4 Includes
#include "Kismet/GameplayStatics.h"

// Library Includes

// Local Includes

// Generated Include
#include "QuarrieDamageBPLibrary.generated.h"

// Prototypes

UENUM( BlueprintType )
enum class EGraphicsOption : uint8
{
	GRAPHICS_ALL_OPTIONS = 0		UMETA( DisplayName = "Set All" ),
	GRAPHICS_ANTI_ALIASING			UMETA( DisplayName = "Anti-Aliasing" ),
	GRAPHICS_EFFECTS				UMETA( DisplayName = "Effects" ),
	GRAPHICS_POST_PROCESSING		UMETA( DisplayName = "Post Processing" ),
	GRAPHICS_RESOLUTION				UMETA( DisplayName = "Resolution Scale" ),
	GRAPHICS_SHADOW					UMETA( DisplayName = "Shadows" ),
	GRAPHICS_TEXTURE				UMETA( DisplayName = "Textures" ),
	GRAPHICS_VIEW_DISTANCE			UMETA( DisplayName = "View Distance" )
};

UCLASS()
class UQuarrieDamageBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
		
	// Damage Helper Functions
	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Damage", meta = ( WorldContext = "WorldContextObject", AutoCreateRefTerm = "IgnoreActors" ) )
	static bool QApplyRadialDamage( UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw = false, AActor* DamageCauser = NULL, AController* InstigatedByController = NULL, bool bDoFullDamage = false, ECollisionChannel DamagePreventionChannel = ECC_Visibility );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Damage", meta = ( WorldContext = "WorldContextObject", AutoCreateRefTerm = "IgnoreActors" ) )
	static bool QApplyRadialDamageWithFalloff( UObject* WorldContextObject, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw = false, AActor* DamageCauser = NULL, AController* InstigatedByController = NULL, ECollisionChannel DamagePreventionChannel = ECC_Visibility );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Damage", meta = ( WorldContext = "WorldContextObject", AutoCreateRefTerm = "IgnoreActors" ) )
	static bool QApplyConicDamageWithFalloff( UObject* WorldContextObject, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageAngle, FVector Direction, float DamageFalloff, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw = false, AActor* DamageCauser = NULL, AController* InstigatedByController = NULL, ECollisionChannel DamagePreventionChannel = ECC_Visibility );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Damage", meta = ( WorldContext = "WorldContextObject", AutoCreateRefTerm = "IgnoreActors" ) )
	static bool QApplyConicDamage( UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageDistance, float DamageAngle, FVector Direction, TSubclassOf< class UDamageType > DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDebugDraw = false, AActor* DamageCauser = NULL, AController* InstigatedByController = NULL, bool bDoFullDamage = false, ECollisionChannel DamagePreventionChannel = ECC_Visibility );

	// Key Bind Helper Functions
	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Key Binding" )
	static bool RebindAxisKey( const FString& _krsAxisName, const FKey& _krOldKey, const FKeyEvent& _krNewKey, FKey& _rNewKeyOut );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Key Binding" )
	static bool RebindActionKey( const FString& _krsActionName, const FKey& _krOldKey, const FKeyEvent& _krNewKey, FKey& _rNewKeyOut );

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Key Binding" )
	static void GetAllAxisKeyBindings( TArray<FInputAxisKeyMapping>& _rBindings );

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Key Binding" )
	static void GetAllActionKeyBindings( TArray< FInputActionKeyMapping >& _rBindings );

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Key Binding" )
	static void SplitInputAxisMapping( FInputAxisKeyMapping _Input, FName& _NameOut, FKey& _KeyOut, float& _ScaleOut );

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Key Binding" )
	static void SplitInputActionMapping( FInputActionKeyMapping _Input, FName& _NameOut, FKey& _KeyOut );

	// Graphics and Rendering Helper Functions
	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Rendering" )
	static UGameUserSettings* GetGameUserSettings();

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Rendering" )
	static bool GetAllResolutions( TArray<FString>& _rsResolutionOut );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Rendering" )
	static bool SetScreenResolution( const int32 _kiWidth, const int32 _kiHeight );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Rendering" )
	static bool ToggleFullScreen();

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Rendering" )
	static bool IsFullscreen( bool& _bReturn );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Rendering" )
	static bool ApplySettings();

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Rendering" )
	static bool GetQualityLevels( int32& _riAntiAliasing,
								  int32& _riEffects,
								  int32& _riPostProcessing,
								  int32& _riResolution,
								  int32& _riShadow,
								  int32& _riTexture,
								  int32& _riViewDistance );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Rendering" )
	static bool GetSingleQualityLevel( const EGraphicsOption& _kreOption,
									   int32& _riQualityValue );

	UFUNCTION( BlueprintPure, Category = "QuarrieLibrary|Rendering" )
	static bool GetAllOptionsValue( int32& _riAllOptionsValue );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Rendering" )
	static bool SetQualityLevels( const int32& _kriAntiAliasing,
								  const int32& _kriEffects,
								  const int32& _kriPostProcessing,
								  const int32& _kriResolution,
								  const int32& _kriShadow,
								  const int32& _kriTexture,
								  const int32& _kriViewDistance );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Rendering" )
	static bool SetSingleQualityLevel( const EGraphicsOption& _kreOption,
									   const int32& _kriNewValue );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Sound" )
	static bool GetVolumeLevel( FString _SoundClassName,
								float& _rfVolume );

	UFUNCTION( BlueprintCallable, Category = "QuarrieLibrary|Sound" )
	static bool SetVolumeLevel( FString _SoundClassName,
							    float _fNewVolume );
};