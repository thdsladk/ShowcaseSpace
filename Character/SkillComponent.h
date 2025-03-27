// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnClickBtnQ);
//DECLARE_MULTICAST_DELEGATE(FOnClickBtnW);
//DECLARE_MULTICAST_DELEGATE(FOnClickBtnE);
//DECLARE_MULTICAST_DELEGATE(FOnClickBtnR);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill, uint8);

class ASkillCommandBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	enum ESkill : uint8
	{
		Skill_Q,
		Skill_W,
		Skill_E,
		Skill_R,
		END

	};
public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void Click_Q();
	void Click_W();
	void Click_E();
	void Click_R();
	void Click_Btn(uint8 Btn);

	//FOnClickBtnQ m_OnClickBtnQ;
	//FOnClickBtnW m_OnClickBtnW;
	//FOnClickBtnE m_OnClickBtnE;
	//FOnClickBtnR m_OnClickBtnR;
	//FOnSkill m_OnSkill;

	void Cancel();

	UFUNCTION()
	void OnSkillMontageEnded();
	UFUNCTION()
	void OnSkillMontagePoint();


protected:
	UPROPERTY()
	TWeakObjectPtr<class AMyTest_TopDownCharacter> m_pOwner;
	UPROPERTY()
	TWeakObjectPtr<class UMyAnimInstance> m_pAnimInstance;
	UPROPERTY()
	TWeakObjectPtr<class UMyStatComponent> m_pStatComp;
		
	ESkill m_CurrentSkill = ESkill::END;

	//UPROPERTY()
	//class UNiagaraSystem* m_SwordTrail;

	TArray<ASkillCommandBase*> m_SkillList;
};
