// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillCommandBase.h"
#include "Skill_WaterBall.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API ASkill_WaterBall : public ASkillCommandBase
{
	GENERATED_BODY()
public:
	ASkill_WaterBall();

	virtual void UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)override;
	virtual void StopSkill()override;

	virtual void PlaySkill()override;
	virtual void CheckSkill()override;
	virtual void OnEffect()override;

	virtual void OnSkillMontageEnded()override;
	virtual void OnSkillMontagePoint()override;

protected:
	// Particle
	UPROPERTY()
	TSubclassOf<AActor> m_ThunderBall;
};
