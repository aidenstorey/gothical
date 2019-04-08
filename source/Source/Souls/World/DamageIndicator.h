// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageIndicator.generated.h"

UCLASS()
class SOULS_API ADamageIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageIndicator( const FObjectInitializer& ObjectInitializer );

	UFUNCTION( BlueprintCallable, Category = "Initialise" )
	void Initialise( const float& _krfDamageAmount );

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
private:

public:
protected:
private:
	UPROPERTY( VisibleAnywhere, Category = "DamageText" )
	class UTextRenderComponent* m_pDamageText;
	
};
