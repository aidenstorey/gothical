// Copyright 2015 Half Tower, Inc. All Rights Reserved.

#pragma once

// UE4 Includes
#include "Object.h"

// Library Includes

// Local Includes
#include "SkillType.h"

//Generated Include
#include "SkillManager.generated.h"

// Prototypes
USTRUCT( BlueprintType )
struct FSkillClass
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		ESkillClassType m_eType;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		class UClass* m_pSkill;
};

USTRUCT( BlueprintType )
struct FSkillManagerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		TArray< FSkillClass > m_arrSkillClasses;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Skills )
		int32 m_iPassiveMaxCount;
};

UCLASS( Blueprintable )
class USkillManager : public UObject
{
	GENERATED_BODY()

	// Member Functions
public:
	// Constructors
	USkillManager();

	// Other
	void Initialise();
	class USkillBase* RequestSkill();
	class USkillBase* CreateSkill( ESkillClassType _eType );
	class USkillBase* CreateRandomSkill();
	class USkillBase* CreateCustomPassive( int32 _iHealth, int32 _iDamage, int32 _iAttackSpeed, int32 _iMovementSpeed );

	bool IsSkillAvailable( ESkillClassType _eType );

protected:
private:

	// Member Variables
public:
protected:
private:
	UPROPERTY() TMap< ESkillClassType, class UClass* > m_mapSkillClasses;
	UPROPERTY() int32 m_iPassiveMaxCount;

	UPROPERTY() TArray< class UClass* > m_arrSkillClasses;
	UPROPERTY() int32 m_iNextSkillIndex;


};