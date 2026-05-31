// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceBase.h"
#include "GASAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASAnimInstanceBase : public UAnimInstanceBase
{
	GENERATED_BODY()
public:
	UGASAnimInstanceBase();
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
};
