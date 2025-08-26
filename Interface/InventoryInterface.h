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

/**		( 임시 ) 
 *		현재 UseItem은 캐릭터에서 인벤토리 컴포넌트로 호출되는 구조로 잘 쓰이는데 ...
 *			Equip은 인벤토리 컴포넌트에서 다시 캐릭터의 Equip을 호출하는 구조라서 
			이걸(Equip) 바꿔야한다.
 */
class MYTEST_TOPDOWN_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// General Section
	virtual const int32 GetItemType(int32 Index) = 0;

	// Item Section
	//virtual void TakeItem(class UMyItemData* InItemData) = 0;
	virtual void UseItem(int32 Index)					=0 ;
	virtual void DropItem(int32 Index, FVector Pos)		=0 ;

	// Equipment Section
	virtual bool Equip(uint8 Part) = 0;
	virtual bool UnEquip(uint8 Part) = 0;

};
