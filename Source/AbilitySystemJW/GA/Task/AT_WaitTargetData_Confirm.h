// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AT_WaitTargetData_Confirm.generated.h"

class ATA_Skill;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConfirmCheckResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConfirmCheckCancelDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAT_WaitTargetData_Confirm : public UAbilityTask_WaitTargetData
{
	GENERATED_BODY()
public:
	UAT_WaitTargetData_Confirm();


	static UAT_WaitTargetData_Confirm* WaitTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType, TSubclassOf<ATA_Skill> InTargetClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FConfirmCheckResultDelegate OnComplete;
	UPROPERTY(BlueprintAssignable)
	FConfirmCheckCancelDelegate OnCancel;

protected:
	UPROPERTY()
	TObjectPtr<ATA_Skill> m_SpawnedTargetActor;

};
