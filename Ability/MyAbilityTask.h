// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyAbilityTask.generated.h"

class UMyAbilityInstance;
/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyAbilityTask : public UObject
{
	GENERATED_BODY()
public:
    virtual void Start(UMyAbilityInstance* Owner) {}
    virtual void Tick(float DeltaTime) {}
    virtual void Cancel() {}
    virtual bool WantsTick() const { return false; }


    DECLARE_MULTICAST_DELEGATE(FOnTaskCompleted);
    FOnTaskCompleted m_OnCompleted;
protected:
    UPROPERTY() 
    UMyAbilityInstance* m_pOwner = nullptr;

};
