// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/SkillEnum.h"
#include "Header/BehaviorEnum.h"
#include "SkillComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEndSkill, uint8);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill, uint8);

class ACharacterBase;
class UAnimInstanceBase;
class UMyStatComponent;
class ASkillCommandBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseSkill(uint8 Btn);
	void CancelSkill();

	FEndSkill m_EndSkill;
	FOnSkill m_OnSkill;

	UFUNCTION()
	void OnSkillMontageEnded();
	UFUNCTION()
	void OnSkillMontagePoint();

protected:
	UPROPERTY()
	TWeakObjectPtr<ACharacterBase> m_pOwner;
	UPROPERTY()
	TWeakObjectPtr<UAnimInstanceBase> m_pAnimInstance;
	UPROPERTY()
	TWeakObjectPtr<UMyStatComponent> m_pStatComp;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")		// ESKill을 에디터에서 사용 못한다.
	ESkill m_CurrentSkill = ESkill::End;
	EBehaviorState m_PreBehaviorState;

	TArray<TWeakObjectPtr<ASkillCommandBase>> m_SkillList;
};
