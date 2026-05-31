// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AT_WaitTargetData_Skill.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillCheckResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillCheckCancelDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);


class ATA_Skill;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAT_WaitTargetData_Skill : public UAbilityTask_WaitTargetData
{
	GENERATED_BODY()
public:
	UAT_WaitTargetData_Skill();

	static UAT_WaitTargetData_Skill* WaitTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType,TSubclassOf<ATA_Skill> InTargetClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FSkillCheckResultDelegate OnComplete;
	UPROPERTY(BlueprintAssignable)
	FSkillCheckCancelDelegate OnCancel;

protected:
	UPROPERTY()
	TSubclassOf<ATA_Skill> m_TargetActorClass;
	UPROPERTY()
	TObjectPtr<ATA_Skill> m_SpawnedTargetActor;


};
