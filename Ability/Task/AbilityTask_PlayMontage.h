// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAbilityTask.h"
#include "AbilityTask_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UAbilityTask_PlayMontage : public UMyAbilityTask
{
	GENERATED_BODY(Blueprintable)
public:
    virtual void Start(UMyAbilityInstance* Owner)override;
    virtual void Tick(float DeltaTime)override;
    virtual void Cancel()override;
    virtual bool WantsTick() const override;

};
