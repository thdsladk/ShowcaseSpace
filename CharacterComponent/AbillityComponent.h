// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/SkillEnum.h"
#include "Header/BehaviorEnum.h"
#include "AbillityComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbillity, uint8);
DECLARE_MULTICAST_DELEGATE_OneParam(FEndAbillity, uint8);

class ACharacterBase;
class UAnimInstanceBase;
class UMyStatComponent;
class ASkillCommandBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UAbillityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbillityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void UseAbillity(uint8 Btn);
	void CancelAbillity();

public:
	FOnAbillity m_OnAbillity;
	FEndAbillity m_EndAbillity;

	UFUNCTION()
	void OnAbillityMontageEnded();
	UFUNCTION()
	void OnAbillityMontagePoint();

protected:
	UPROPERTY()
	TWeakObjectPtr<ACharacterBase> m_pOwner;
	UPROPERTY()
	TWeakObjectPtr<UAnimInstanceBase> m_pAnimInstance;
	UPROPERTY()
	TWeakObjectPtr<UMyStatComponent> m_pStatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")		// ESKill을 에디터에서 사용 못한다.
	ESkill m_CurrentSkill = ESkill::End;
	EBehaviorState m_PreBehaviorState;

	TArray<TWeakObjectPtr<ASkillCommandBase>> m_SkillList;
};
