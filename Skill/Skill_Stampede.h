// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillCommandBase.h"
#include "Skill_Stampede.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API ASkill_Stampede : public ASkillCommandBase
{
	GENERATED_BODY()
public:
	ASkill_Stampede();

	virtual void UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)override;
	virtual void StopSkill()override;

	virtual void PlaySkill()override;
	virtual void CheckSkill()override;
	virtual void OnEffect()override;

	virtual void OnSkillMontageEnded()override;
	virtual void OnSkillMontagePoint()override;

	virtual void SkillTick()override;

	void PlaySkill_Stampede_Knockup();
	void CheckSkill_Stampede_Knockup();

	void Effect_Thunder();
	void Effect_Dust();




protected:
	// Skill Cooltime 
	FTimerHandle m_Timer_Stampede;

	// Particle
	UPROPERTY()
	TArray<TSubclassOf<AActor>> m_ThunderList;

	bool m_bStampede = false;


	UPROPERTY()
	class UNiagaraSystem* m_pDust;



};
