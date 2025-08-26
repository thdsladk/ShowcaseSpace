// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class AMyInteractable;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyInteraction() = 0;
	virtual void CheckForInteractable()=0;
	UFUNCTION()
	virtual bool HasCurrentInteractable()=0;
	UFUNCTION()
	virtual void SetCurrentInteractable(AMyInteractable* Interactable)=0;
};
