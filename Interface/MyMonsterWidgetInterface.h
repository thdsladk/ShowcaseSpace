// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyMonsterWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyMonsterWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API IMyMonsterWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupMonsterWidget(class UMyUserWidget* InUserWidget) = 0;
};
