// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/DataStruct.h"
#include "MyInventoryComponent.generated.h"

/*
	FItemRuntimeData는 외부에서 들어오는 아이템 데이터 형태다.
	밖으로 반환하는 사용되는 값도 FItemRuntimeData으로 반환한다.

	FItemData는 오직 아이템에 대한 순수 정보를 뽑을 때만 쓴다.


	(중요) 슬롯이 아니라 인벤토리 시스템에서 인벤토리 위젯을 갱신 해주도록
	구성해야한다. 
*/



DECLARE_MULTICAST_DELEGATE(FOnUpdateWidget);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateSlotWidget,int32);

// 하나의 함수만 바인딩 하기위하여...
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

	// 아이템 조작 부분 
	FItemRuntimeData PopItem(int32 Index);

	bool TakeItem(FItemRuntimeData& ItemData, int32 Index);
	bool TakeItem(FItemRuntimeData& ItemData);

	//TArray<int32> UseItem(int32 Index);
	FItemRuntimeData DropItem(int32 Index);

	// 아이템 병합,분리
	bool MergeItems(FItemRuntimeData& ItemData, int32 To);
	bool MergeItems(int32 From,int32 To);
	bool SlitItems(int32 From, int32 To);

	void SortInventory();

	// 아이템 이동 관련 함수
	void SwapInventory(int32 From, int32 To);

	// Equipment Section
	
	bool Equip(int32 FromInven , int32 ToEquip = -1);
	bool UnEquip(int32 FromEquip,int32 ToInven = -1);
	
	FORCEINLINE uint32 GetInventorySize() { return (m_Row * m_Column); }
	FORCEINLINE uint32 GetTotalSize() { return m_Inventory.Num(); }

#pragma region Inner Function
protected:
	// 내부용 아이템 제어 함수들
	bool AddCount(int32 Index,int32 Count =1);
#pragma endregion

protected:
	// Equipment ( 내부용 ) 
	/*
	bool AttachEquipment(int32 Index);
	bool DetachEquipment(int32 Index);
	*/
	bool InitEquipments();
	bool InitEquipment(int32 Index);
private:
	// 내부 계산용 함수들 
	int32 SearchPedingSlot(int32 ID, int32 Amount = 1);

#pragma region Delegate
public:
	FOnUpdateWidget m_UpdateWidget;				// 인벤토리 위젯이랑 바인딩하자
	FOnUpdateSlotWidget m_UpdateSlotWidget;		
	FOnSwapWidget m_SwapWidget;					// 교환 전용 델리게이트[ 싱글 델리게이트 ] 
	
#pragma endregion

protected:

	UPROPERTY(EditAnywhere,Category = Container)
	TArray<FInventoryData>				m_Inventory;
	/*
	* 아이템 리스트 + 장비 리스트 순으로 인벤토리는 구성되어있다. 
	*/
	UPROPERTY(EditAnywhere, Category = Container)
	TArray<TWeakObjectPtr<USkeletalMeshComponent>> m_EquipmentMeshComp;
	
	const int32 m_ItemFirstIndex = 6;
	TArray<FName> m_PartTypeList;


	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;
};
