// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SkillCommandInterface.h"
#include "SkillCommandBase.generated.h"


DECLARE_MULTICAST_DELEGATE(FStopSkill);

UCLASS()
class MYTEST_TOPDOWN_API ASkillCommandBase : public AActor , public ISkillCommandInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillCommandBase();
	FORCEINLINE const bool IsCanEverTick() { return m_bCanEverTick; }

	virtual void InitSkill(const FSkillData* SkillData, APawn* Pawn, UAnimInstanceBase* Anim, UMyStatComponent* StatComp)override {}
	virtual bool UseSkill()override { return m_bEnableSkill; }
	virtual void StopSkill()override {}
	virtual void StoppedSkill(UAnimMontage* Montage, bool bInterrupted)override {}

	virtual void PlaySkill()override {}
	virtual void CheckSkill()override {}
	virtual void OnEffect()override {}
	
	virtual void OnSkillMontageEnded()override {}	//��ų�� �ѹ��� �ϳ����� �����ٰ� ��������.
	virtual void OnSkillMontagePoint()override{}	//��ų�� �ѹ��� �ϳ����� �����ٰ� ��������.

#pragma region Timer Section		// TimerHandle ��ġ�� ����ؼ� �������̽��� �ƴ϶� ���� �־���.
	virtual void SkillTick(float DeltaTime) {}
	virtual void OnCoolDown() { m_bEnableSkill = true; }
#pragma endregion 
public:
	FStopSkill m_StopSkill;
protected:
	TWeakObjectPtr<APawn> m_pOwner;
	TWeakObjectPtr<UAnimInstanceBase> m_pAnimInstance;
	TWeakObjectPtr<UMyStatComponent> m_pStatComp;

	FTimerHandle m_SkillTimer;
	FTimerHandle m_CoolDownTimer;

	const FSkillData* m_SkillData;


	// ��ų���� ���忡 �������� ���� ���̶� ��ų ������Ʈ�� ƽ�� �����ͼ� ����.
	uint8 m_bCanEverTick : 1;

	uint8 m_bEnableSkill : 1;

};
