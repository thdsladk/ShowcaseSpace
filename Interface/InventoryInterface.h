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

/**		( �ӽ� ) 
 *		���� UseItem�� ĳ���Ϳ��� �κ��丮 ������Ʈ�� ȣ��Ǵ� ������ �� ���̴µ� ...
 *			Equip�� �κ��丮 ������Ʈ���� �ٽ� ĳ������ Equip�� ȣ���ϴ� ������ 
			�̰�(Equip) �ٲ���Ѵ�.
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
