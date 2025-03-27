// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//virtual void TakeItem(class UMyItemData* InItemData) = 0;
	virtual void UseItem(int32 Index)					=0 ;
	virtual void DropItem(int32 Index, FVector Pos)		=0 ;
};
