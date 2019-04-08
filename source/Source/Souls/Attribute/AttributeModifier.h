// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes

//Generated Include
#include "AttributeModifier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FModifierAltered );

// Prototypes
USTRUCT( BlueprintType )
struct FAttributeModifierDefinition
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Souls )
	FString m_tName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Souls )
	int32 m_iAmount;
};

UCLASS()
class UAttributeModifier : public UObject
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	UAttributeModifier();

	// Accessors
	FString GetAttributeName() const;

	int32 GetAmount() const;
	void SetAmount( const int32& _kriAmount );

	FModifierAltered& GetOnAlteredDelegate();

	// Other
	void Initialise( const FAttributeModifierDefinition& _tDefinition );
	void Initialise( const FString& _krstrAttributeName, const int32& _kriAmount );

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() FString m_strAttributeName;
	UPROPERTY() int32 m_iAmount;

	FModifierAltered m_tOnAltered;
};