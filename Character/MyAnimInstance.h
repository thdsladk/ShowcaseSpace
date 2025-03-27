// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AnimInstanceBase.h"
#include "MyAnimInstance.generated.h"


/**
 * 
 */


UCLASS()
class MYTEST_TOPDOWN_API UMyAnimInstance : public UAnimInstanceBase
{
	GENERATED_BODY()
public:
	UMyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:

};
