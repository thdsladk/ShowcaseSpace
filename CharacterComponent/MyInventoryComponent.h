// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/DataStruct.h"
#include "MyInventoryComponent.generated.h"

/*
	FItemRuntimeData�� �ܺο��� ������ ������ ������ ���´�.
	������ ��ȯ�ϴ� ���Ǵ� ���� FItemRuntimeData���� ��ȯ�Ѵ�.

	FItemData�� ���� �����ۿ� ���� ���� ������ ���� ���� ����.


	(�߿�) ������ �ƴ϶� �κ��丮 �ý��ۿ��� �κ��丮 ������ ���� ���ֵ���
	�����ؾ��Ѵ�. 
*/



DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateSlotWidget,int32);

// �ϳ��� �Լ��� ���ε� �ϱ����Ͽ�...
DECLARE_DELEGATE_TwoParams(FOnSwapWidget,int32,int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInventoryComponent();
	virtual void SetupData(FString TableRawName);

protected:
	virtual void InitializeComponent() override;

public:	
	UTexture2D* GetThumbnail(int32 Index); 
	const int32 GetItemCount(int32 Index);
	FString GetItemName(int32 Index);
	const int32 GetItemType(int32 Index);
	const FBaseStatusData GetItemStat(int32 Index);
	uint8 UseItem(FBaseStatusData& OutStatus, int32 Index);
	bool UseEquipment(FBaseStatusData& OutStatus, int32 Index);

	TSoftObjectPtr<USkeletalMesh> GetSkeletalMesh_Equipment(int32 Part);

	// ������ ���� �κ� 
	FItemRuntimeData PopItem(int32 Index);

	bool TakeItem(FItemRuntimeData& ItemData, int32 Index);
	bool TakeItem(FItemRuntimeData& ItemData);

	//TArray<int32> UseItem(int32 Index);
	FItemRuntimeData DropItem(int32 Index);

	// ������ ����,�и�
	bool MergeItems(FItemRuntimeData& ItemData, int32 To);
	bool MergeItems(int32 From,int32 To);
	bool SlitItems(int32 From, int32 To);

	void SortInventory();

	// ������ �̵� ���� �Լ�
	void SwapInventory(int32 From, int32 To);

	// Equipment Section
	
	bool Equip(int32 FromInven , int32 ToEquip = -1);
	bool UnEquip(int32 FromEquip,int32 ToInven = -1);
	
	FORCEINLINE uint32 GetInventorySize() { return (m_Row * m_Column); }
	FORCEINLINE uint32 GetTotalSize() { return m_Inventory.Num(); }

#pragma region Inner Function
protected:
	// ���ο� ������ ���� �Լ���
	bool AddCount(int32 Index,int32 Count =1);
#pragma endregion

protected:
	// Equipment ( ���ο� ) 
	/*
	bool AttachEquipment(int32 Index);
	bool DetachEquipment(int32 Index);
	*/
	bool InitEquipments();
	bool InitEquipment(int32 Index);
private:
	// ���� ���� �Լ��� 
	int32 SearchPedingSlot(int32 ID, int32 Amount = 1);

#pragma region Delegate
public:
	FOnUpdateWidget m_UpdateWidget;				// �κ��丮 �����̶� ���ε�����
	FOnUpdateSlotWidget m_UpdateSlotWidget;		
	FOnSwapWidget m_SwapWidget;					// ��ȯ ���� ��������Ʈ[ �̱� ��������Ʈ ] 
	
#pragma endregion

protected:

	UPROPERTY(EditAnywhere,Category = Container)
	TArray<FInventoryData>				m_Inventory;
	/*
	* ������ ����Ʈ + ��� ����Ʈ ������ �κ��丮�� �����Ǿ��ִ�. 
	*/
	UPROPERTY(EditAnywhere, Category = Container)
	TArray<TWeakObjectPtr<USkeletalMeshComponent>> m_EquipmentMeshComp;
	
	const int32 m_ItemFirstIndex = 6;
	TArray<FName> m_PartTypeList;


	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;
};
