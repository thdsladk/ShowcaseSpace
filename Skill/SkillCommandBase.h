// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SkillCommandInterface.h"
#include "SkillCommandBase.generated.h"

UCLASS()
class MYTEST_TOPDOWN_API ASkillCommandBase : public AActor , public ISkillCommandInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillCommandBase();

	virtual void UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)override {}
	virtual void StopSkill()override {}

	virtual void PlaySkill()override {}
	virtual void CheckSkill()override {}
	virtual void OnEffect()override {}
	
	virtual void OnSkillMontageEnded()override {}	//��ų�� �ѹ��� �ϳ����� �����ٰ� ��������.
	virtual void OnSkillMontagePoint()override{}	//��ų�� �ѹ��� �ϳ����� �����ٰ� ��������.

	virtual void SkillTick() {}
protected:
	TWeakObjectPtr<class APawn> m_pOwner;
	TWeakObjectPtr<class UMyAnimInstance> m_pAnimInstance;
	TWeakObjectPtr<class UMyStatComponent> m_pStatComp;

	FTimerHandle m_Timer_Tick;	

};
