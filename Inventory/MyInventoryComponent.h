// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyInventoryComponent.generated.h"

// define 
#define EmptyIndex 100


DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);
// �ϳ��� �Լ��� ���ε� �ϱ����Ͽ�...
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

	// ������ ���� �κ� 
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
	FOnUpdateWidget m_UpdateWidget;	// �κ��丮 �����̶� ���ε�����
	FOnSwapWidget m_SwapWidget;		// ��ȯ ���� ��������Ʈ[ �̱� ��������Ʈ ] 
protected:

	TArray<TArray<class AMyItem*>> m_Inventory;
	int32 m_RefEquipment[5]{ EmptyIndex,EmptyIndex,EmptyIndex,EmptyIndex,EmptyIndex };

	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;
};
