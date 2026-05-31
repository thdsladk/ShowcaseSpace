// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_AbilityBase.h"
#include "GA_PassiveAbilityBase.generated.h"

/**
 * 패시브 스킬용 베이스 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGA_PassiveAbilityBase : public UGA_AbilityBase
{
	GENERATED_BODY()

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)override;

};
