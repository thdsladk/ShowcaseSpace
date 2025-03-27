// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillCommandBase.h"
#include "Skill_DoublePain.generated.h"

UCLASS()
class MYTEST_TOPDOWN_API ASkill_DoublePain : public ASkillCommandBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkill_DoublePain();

	virtual void UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)override;
	virtual void StopSkill()override;

	virtual void PlaySkill()override;
	virtual void CheckSkill()override;
	virtual void OnEffect()override;

	virtual void OnSkillMontageEnded()override;
	virtual void OnSkillMontagePoint()override;


protected:

	// Skill Cooltime 
	FTimerHandle m_Timer_DoublePain;
};
