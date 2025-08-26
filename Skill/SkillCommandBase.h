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
	
	virtual void OnSkillMontageEnded()override {}	//스킬은 한번에 하나씩만 써진다고 전제하자.
	virtual void OnSkillMontagePoint()override{}	//스킬은 한번에 하나씩만 써진다고 전제하자.

#pragma region Timer Section		// TimerHandle 위치를 고려해서 인터페이스가 아니라 여기 넣었다.
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


	// 스킬들은 월드에 스폰되지 않을 것이라서 스킬 컴포넌트의 틱을 가져와서 쓰자.
	uint8 m_bCanEverTick : 1;

	uint8 m_bEnableSkill : 1;

};
