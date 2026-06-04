// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AT_PlayDodgeAndWait.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UAT_PlayDodgeAndWait : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
public:
	UAT_PlayDodgeAndWait();

	static UAT_PlayDodgeAndWait* CreateDodgeAndWaitProxy(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, 
		bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

public:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	virtual void TickTask(float DeltaTime) override;

protected:
	void TurnOffTick() { bTickingTask = false; }
};
