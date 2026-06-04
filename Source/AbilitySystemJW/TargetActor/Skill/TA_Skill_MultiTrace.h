// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetActor/TA_Skill.h"
#include "TA_Skill_MultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_Skill_MultiTrace : public ATA_Skill
{
	GENERATED_BODY()
public:
	ATA_Skill_MultiTrace();
protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
