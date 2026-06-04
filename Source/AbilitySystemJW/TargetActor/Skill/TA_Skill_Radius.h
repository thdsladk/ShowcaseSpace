// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetActor/TA_Skill.h"
#include "TA_Skill_Radius.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_Skill_Radius : public ATA_Skill
{
	GENERATED_BODY()
public:
	ATA_Skill_Radius();

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
