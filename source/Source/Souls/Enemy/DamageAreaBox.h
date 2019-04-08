// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Components/BoxComponent.h"

// Library Includes

// Local Includes

// Generated Include
#include "DamageAreaBox.generated.h"

// Prototypes

UCLASS( Blueprintable, BlueprintType, ClassGroup = "QuarrieDamage", meta = ( BlueprintSpawnableComponent ) )
class UDamageAreaBox : public UBoxComponent
{
	GENERATED_BODY()

	// Member Functions
public:
	// Sets the default values for this component's properties
	UDamageAreaBox( const FObjectInitializer& ObjectInitializer );

	// Called when the game starts
	virtual void InitializeComponent() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION()
	void OnOverlapBegin( class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

protected:

private:

	// Member Variables
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Damage Area" )
		float DamageAmount = 0.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Damage Area" )
		float CoolDown = 1.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Damage Area" )
		TSubclassOf< UDamageType > DamageType;

protected:

private:
	float m_fCoolDown = 0.0f;
	bool  m_bActive{ false };
};
