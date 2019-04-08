// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes

// Library Includes

// Local Includes
#include "../../Base/CharacterBase.h"

// Generated Include
#include "CharacterPlayer.generated.h"

// Prototypes
UCLASS( Blueprintable )
class ACharacterPlayer : public ACharacterBase
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	ACharacterPlayer( const FObjectInitializer& _krtInitialiser );

	// Accessors
	UFUNCTION( BlueprintPure, Category = "Accessors" ) class UCameraComponent* GetCamera() const;
	UFUNCTION( BlueprintPure, Category = "Accessors" ) class USpringArmComponent* GetCameraBoom() const;
	UFUNCTION( BlueprintPure, Category = "Accessors" ) class UBoxComponent* GetRotationCollider() const;
	UFUNCTION( BlueprintPure, Category = "Accessors" ) class UCapsuleComponent* GetPickupArea() const;

	// Other
	UFUNCTION() void BeginRegenParticles();
	UFUNCTION() void EndRegenParticles();

	// UE4 Overrides
	virtual float TakeDamage( float _fDamageAmount, struct FDamageEvent const& _krtDamageEvent, class AController* _pEventInstigator, AActor* _pDamageCauser ) override;

protected:
private:

	// Member Variables
public:
protected:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Souls General", Meta = ( DisplayName = "Camera" ) ) class UCameraComponent* m_pCamera;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Souls General", Meta = ( DisplayName = "Camera Boom" ) ) class USpringArmComponent* m_pCameraBoom;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Souls General", Meta = ( DisplayName = "Rotation Collider" ) ) class UBoxComponent* m_pRotationCollider;
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Souls General", Meta = ( DisplayName = "Pickup Area" ) ) class UCapsuleComponent* m_pPickupArea;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Souls", Meta = ( Displayname = "Regen Particles" ) ) class UParticleSystem* m_pRegenParticles;

private:
	UParticleSystemComponent* m_pActiveParticles;
};