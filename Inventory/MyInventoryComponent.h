// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyInventoryComponent.generated.h"

// define 
#define EmptyIndex 100


DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);
// 하나의 함수만 바인딩 하기위하여...
DECLARE_DELEGATE_TwoParams(FOnSwapWidget,int32,int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UTexture2D* GetThumbnail(int32 Index); 
	int32 GetItemCount(int32 Index);
	FString GetItemName(int32 Index);

	// 아이템 조작 부분 
	bool PushBackItem(class AMyItem* Item, int32 Index);
	class AMyItem* PopBackItem(int32 Index);

	TArray<int32> UseItem(int32 Index);
	void DropItem(int32 Index,FVector Pos);
	int32 FindAddIndex(int32 ID);

	void SwapInventory(int32 From, int32 To);
	void OnEquipment(int32 Index);
	
	int32 GetEquipmentRef(int32 Index) { return m_RefEquipment[Index]; }
	void SetEquipmentRef(int32 Index, int32 To) { m_RefEquipment[Index] = To; }
	
	int32 GetInventorySize();

public:
	FOnUpdateWidget m_UpdateWidget;	// 인벤토리 위젯이랑 바인딩하자
	FOnSwapWidget m_SwapWidget;		// 교환 전용 델리게이트[ 싱글 델리게이트 ] 
protected:

	TArray<TArray<class AMyItem*>> m_Inventory;
	int32 m_RefEquipment[5]{ EmptyIndex,EmptyIndex,EmptyIndex,EmptyIndex,EmptyIndex };

	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;
};
