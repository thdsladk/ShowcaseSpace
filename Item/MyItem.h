// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyInteractable.h"
#include "Header/DataStruct.h"
#include "MyItem.generated.h"

/*
	해당 액터는 맵에 존재하기 위해서 만든 용도이다. 

	아이템과 장비를 나눌 필요가 없어서 합쳤다 .
*/

UCLASS()
class MYTEST_TOPDOWN_API AMyItem : public AMyInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

#pragma region Data Function
public:
#if WITH_EDITOR
	// EditorTime Data
	void InitItem();
#endif
	// Runtime Data
	void SetItem(int32 ID, uint8 Count = 1);
	bool InitData(FItemRuntimeData& RuntimeData);
#pragma endregion

public:
	//FORCEINLINE const int32 GetID() { return m_ItemInfo.ItemStaticData.ID;
	//}
	//FORCEINLINE const int32 GetType() { return m_ItemInfo.ItemStaticData.Type; }
	//FORCEINLINE FString GetName() { return m_ItemInfo.ItemStaticData.Name; }
	//FORCEINLINE FString GetDesc() { return m_ItemInfo.ItemStaticData.Description; }
	//FORCEINLINE const int32 GetMaxCount() { return m_ItemInfo.ItemStaticData.MaxCount; }
	//FORCEINLINE const int32 GetScale() { return m_ItemInfo.ItemStaticData.Scale; }
	//FORCEINLINE UTexture2D* GetThumbnail() { return m_ItemInfo.ItemStaticData.Thumbnail; }
	//FORCEINLINE UStaticMesh* GetItemMesh() { return m_ItemInfo.ItemStaticData.ItemMesh; }
	//FORCEINLINE USkeletalMesh* GetEquipmentMesh() { return m_ItemInfo.ItemStaticData.EquipmentMesh; }
	//FORCEINLINE const FBaseStatusData GetItemStatData() { return m_ItemInfo.ItemStaticData.Status;}
	//FORCEINLINE const int32 GetCount() { return m_ItemInfo.Count; }ㄴ
	FORCEINLINE FItemRuntimeData& GetItemData(){ return m_ItemInfo;}
	FORCEINLINE void PopItemData(FItemRuntimeData& OutData) { OutData = m_ItemInfo; m_ItemInfo = FItemRuntimeData{}; }

public:
	virtual void Init()override;
	void OnInteract();
	virtual void Interact_Implementation()override;
	virtual void OnPickedUp();
	virtual void Replace(const FVector Pos)override;


	virtual void ResetObject()override;
	virtual bool RemoveObject();

#pragma region Poolable Interface
public:
	virtual bool IsUseableObject()override{ return AActor::IsHidden(); }
	virtual void ApplyHidden(const bool bHide)override;
	virtual void AcquireObject()override;
	virtual void ReleaseObject()override;
#pragma endregion

// 아이템이 상속 받지 않아도 될듯해서 아직 주석처리
/*
protected:
	virtual void OnCharacterBeginOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult
		)override;
	
	virtual void OnCharacterEndOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
*/

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataHandle")
	FDataTableRowHandle m_DataTableRowHandle;

	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FItemRuntimeData m_ItemInfo;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> m_EquipmentMeshComp;

	uint8 m_IsGround :1;
	


	


	



};
