// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/GASAnimInstanceBase.h"
#include "GASPlayerLinked_AnimLayer.generated.h"

class UGASAnimInstanceBase;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API UGASPlayerLinked_AnimLayer : public UGASAnimInstanceBase
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UGASAnimInstanceBase* GetGASAnimInstanceBase() const;
};
