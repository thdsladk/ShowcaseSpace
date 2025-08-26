// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API ITargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const AActor* GetTarget() = 0;
	virtual TArray<TWeakObjectPtr<AActor>>& GetTargets() = 0;
	virtual void AddTarget(AActor* Target) = 0;
	virtual void AddTargets(TArray<AActor*> Targets) = 0;
	virtual void PopTarget() = 0;
	virtual void ClearTargets() = 0;

};
