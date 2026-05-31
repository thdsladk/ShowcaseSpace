// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TargetActor/TA_Trace.h"
#include "TA_SphereMultiTrace.generated.h"

/**
 *		 타겟액터는 효과가 시작되는 지점과 방향을 이미 가지고 있다는 전제가 필요하다. 
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_SphereMultiTrace : public ATA_Trace
{
	GENERATED_BODY()

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
