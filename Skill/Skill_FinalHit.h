// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillCommandBase.h"
#include "Skill_FinalHit.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API ASkill_FinalHit : public ASkillCommandBase
{
	GENERATED_BODY()
public:
	ASkill_FinalHit();

	virtual void UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)override;
	virtual void StopSkill()override;

	virtual void PlaySkill()override;
	virtual void CheckSkill()override;
	virtual void OnEffect()override;

	virtual void OnSkillMontageEnded()override;
	virtual void OnSkillMontagePoint()override;

	virtual void SkillTick()override;

	void Effect_GhostTrail();

private:
	bool SearchActor_Sphere(float Range, float Radius, ECollisionChannel Channel);

protected:
	// Skill Cooltime 
	FTimerHandle m_Timer_FinalHit;

	// Target<Enemy> List
	UPROPERTY()
	TArray<AActor*> m_TargetList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GhostTrailActor")
	TSubclassOf<class AGhostTrail> m_pGhostTrail;

	bool m_bGhostTrail = false;
};
