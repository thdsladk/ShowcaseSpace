// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillCommandInterface.generated.h"

class UMyStatComponent;
class UMyAnimInstance;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillCommandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API ISkillCommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp) = 0;
	virtual void StopSkill() = 0;

	virtual void PlaySkill() = 0;
	virtual void CheckSkill() = 0;
	virtual void OnEffect() = 0;

	virtual void OnSkillMontageEnded() = 0;
	virtual void OnSkillMontagePoint() = 0;

};
