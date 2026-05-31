// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WeaponCollision.generated.h"

class ATA_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAT_WeaponCollision : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAT_WeaponCollision();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForWeaponCollision", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_WeaponCollision* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_Base> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;
	
	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	UFUNCTION()
	void FinishWeaponTask(FGameplayEventData Payload);

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FWeaponResultDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FSendTargetDataDelegate OnSendTargetData;

protected:
	UPROPERTY()
	TSubclassOf<ATA_Base> m_TargetActorClass;

	UPROPERTY()
	TObjectPtr<ATA_Base> m_SpawnedTargetActor;
};
