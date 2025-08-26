// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventoryComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyTest_TopDownGameMode.h"
#include "MyInventoryWidget.h"

//#include "GameFramework/Character.h"
#include "Character/CharacterBase.h"
#include "Header/GlobalEnum.h"
#include "Interface/InventoryInterface.h"
#include "Data/MyGameSubsystem.h"

// define 
#define EmptyIndex 100

constexpr int32 NullIndex = -1;


UMyInventoryComponent::UMyInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	m_Inventory.Init(FInventoryData(), m_ItemFirstIndex + (m_Row * m_Column));


	m_PartTypeList.Init(TEXT(""), m_ItemFirstIndex);
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Head)] = TEXT("Head_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_L)] = TEXT("LeftHand_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Chest)] = TEXT("Chest_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_R)] = TEXT("RightHand_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_L)] = TEXT("LeftFoot_Socket");
	m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_R)] = TEXT("RightFoot_Socket");

	//m_EquipmentMeshComp.Init(nullptr, m_ItemFirstIndex);

}

void UMyInventoryComponent::SetupData(FString TableRawName)
{

	UMyGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (GameSubsystem != nullptr)
	{
		const FEquipmentInfo* EquipmentData = GameSubsystem->GetCharacterInfo_EquipmentInfo(TableRawName);
		if (EquipmentData != nullptr)
		{

			// ��� �����͸� �޾ƿ��� �κ� 
			// �ּ��� ũ�⸦(int32*) ���ؼ� �迭ó�� ����.		
			// �� �̷��� �ϸ� Ȯ���ϰ� ��Ʈũ��� �����;ȿ� ItemID������ ���� ũ�Ⱑ ��ġ�ؾ��Ѵ�.
			// (�ӽ�) const�� ���� �����鼭 ... const_cast�� ĳ�����ؼ� ����ϴµ� ������ ������ ?
			int32* pItemID = const_cast<int32*>(& EquipmentData->HItemID);
			for (int32 Index = 0; Index < static_cast<int32>(EEquipmentPart::End); Index++,pItemID++)
			{
				if (*pItemID == -1) continue;

				auto ItemData = GameSubsystem->GetItemData(*pItemID);

				// ĳ���Ϳ� ��� ���� 
				// ĳ���͸� ������ ���� �������� BeginPlay���Ŀ� SetupData�Լ��� ȣ��Ǿ���Ѵ�.
				IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
				InventoryInterface->Equip(Index);


				if (InitEquipment(Index) == false)
				{
					// Debug
					UE_LOG(LogTemp, Log, TEXT(" %d Index Fail "), Index);
				}

			}

		}

		// ����� �κ��丮�� �����ͷ� ���� �޾ƿ� �κ�


	}

}

/*
void UMyInventoryComponent::BindEquipment(TArray<USkeletalMeshComponent*> EquipmentList)
{
	// InitializeComponent ���Ŀ� ȣ�� �Ǿ�� ��ȿ�ϴ�. [ GetMesh�� ��ȿ�� �������� ] 

	for (int32 Index = 0; Index < m_ItemFirstIndex; Index++)
	{
		if (EquipmentList[Index] != nullptr)
		{
			m_EquipmentMeshComp[Index] = EquipmentList[Index];
			
			if (InitEquipment(Index) == false)
			{
				// Debug
				UE_LOG(LogTemp, Log, TEXT(" %d Index Fail "), Index);
			}
		}
	}
}
*/

void UMyInventoryComponent::InitializeComponent()
{
	// �κ��丮�� �����̳ʴ� ���⼭ �ʱ�ȭ !!!

	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
	if (CharacterMesh == nullptr)
	{
		return;
	}


}

UTexture2D* UMyInventoryComponent::GetThumbnail(int32 Index)
{
	if (m_Inventory[Index].Count <= 0)
		return nullptr;
	return m_Inventory[Index].ItemStaticData.Thumbnail;
}

const int32 UMyInventoryComponent::GetItemCount(int32 Index)
{
	if (m_Inventory[Index].Count <= 0)
		return NULL;
	return m_Inventory[Index].Count;
}

FString UMyInventoryComponent::GetItemName(int32 Index)
{
	if (m_Inventory[Index].Count <= 0)
		return FString(TEXT("Empty"));
	return m_Inventory[Index].ItemStaticData.Name;
}

const int32 UMyInventoryComponent::GetItemType(int32 Index)
{
	if (m_Inventory[Index].Count <= 0)
		return 0;
	return m_Inventory[Index].ItemStaticData.Type;
}

const FBaseStatusData UMyInventoryComponent::GetItemStat(int32 Index)
{
	if (m_Inventory[Index].Count <= 0)
		return FBaseStatusData{};
	return m_Inventory[Index].ItemStaticData.Status;
}

uint8 UMyInventoryComponent::UseItem(FBaseStatusData& OutStatus, int32 Index)
{
	// ��ȯ ���� ������ Ÿ��.
	uint8 ItemType = static_cast<uint8>(m_Inventory[Index].ItemStaticData.Type);
	// �Ҹ� �� �϶��� ��밡�� 
	if (ItemType == static_cast<uint8>(EItemType::ConsumableItem))
	{
		OutStatus = m_Inventory[Index].ItemStaticData.Status;
		if (m_Inventory[Index].Count >= 1)
		{
			// ������ ���ο��� 0�̵Ǹ� �����ϵ��� �Ǿ��ִ�.
			AddCount(Index,-1);
			// Update Widget
			m_UpdateSlotWidget.Broadcast(Index);
		}
		else
		{
			//  0���� ������ ������ �ִ�. // 0�̸� �ƹ��ϵ� ������Ѵ�. 
			return ItemType;
		}
		
	}
	
	return ItemType;
}

bool UMyInventoryComponent::UseEquipment(FBaseStatusData& OutStatus, int32 Index)
{
	// ��ȯ ���� ������ Ÿ��.
		uint8 ItemType = static_cast<uint8>(m_Inventory[Index].ItemStaticData.Type);
	// ��� �� �϶��� ��밡�� 
	if (ItemType == static_cast<uint8>(EItemType::EquipmentItem))
	{
		if (m_Inventory[Index].Count == 1)
		{
			OutStatus = m_Inventory[Index].ItemStaticData.Status;
			return true;
		}
		else
		{
			//  1���� �ƴϸ� ������ �ִ�. 
			return false;
		}

	}

	return false;
}

TSoftObjectPtr<USkeletalMesh> UMyInventoryComponent::GetSkeletalMesh_Equipment(int32 Part)
{
	return m_Inventory[Part].ItemStaticData.EquipmentMesh;
}

bool UMyInventoryComponent::TakeItem(FItemRuntimeData& ItemData, int32 Index)
{
	// ������ Ư�� ���Կ� �������� �о� ������ ����� ����
	if (m_Inventory[Index].Count == 0)
	{

		// ������ �߰�
		m_Inventory[Index].ItemStaticData = ItemData.ItemStaticData;
		m_Inventory[Index].Count = ItemData.Count;

		// ���� ���� ����
		m_UpdateSlotWidget.Broadcast(Index);

		return true;
	}
	else
	{
		// ID Ȯ�� �� �ٸ��� ����.
		if (m_Inventory[Index].ItemStaticData.ID == ItemData.ItemStaticData.ID)
		{
			// ������ ����
			return MergeItems(ItemData, Index);
		}

	}
	return false;

}

bool UMyInventoryComponent::TakeItem(FItemRuntimeData& ItemData)
{
	int32 Index = SearchPedingSlot(ItemData.ItemStaticData.ID, ItemData.Count);
	// ������ ���� ��
	if (Index == -1) { return false; }


	if (m_Inventory[Index].Count == 0)
	{
		// ������ �߰�
		m_Inventory[Index].ItemStaticData = ItemData.ItemStaticData;
		m_Inventory[Index].Count = ItemData.Count;

		// ���� ���� ����
		m_UpdateSlotWidget.Broadcast(Index);
		return true;
	}
	else
	{
		// ������ ����
		return MergeItems(ItemData,Index);
	}

	return false;
}

FItemRuntimeData UMyInventoryComponent::PopItem(int32 Index)
{
	if (m_Inventory[Index].Count <= 0)
		return FItemRuntimeData{};
	else
	{
		FItemRuntimeData RuntimeData;
		RuntimeData.ItemStaticData= m_Inventory[Index].ItemStaticData;
		RuntimeData.Count = m_Inventory[Index].Count;

		// �ʱ�ȭ ����... ����ü�� �ʱ�ȭ ���� �������� �ʾƼ� �׳� 0���� �ʱ�ȭ�� ���̴�. 
		m_Inventory[Index] = FInventoryData{};

		// ���� ���� ����
		m_UpdateSlotWidget.Broadcast(Index);

		// ��ȯ �Ǵ� ���� ������ ������ ��
		return RuntimeData;
	}
}

FItemRuntimeData UMyInventoryComponent::DropItem(int32 Index)
{
	// ���⼭ �� ������ұ� �ܼ��� �������� Pop�ϴ°� �ƴ϶� ����ϴ� ����̶�� ������ ? 
	// ���࿡ �������� ������ ���ֵ� �Ǵ� �Լ���� �ϴ�.
	return PopItem(Index);

}

bool UMyInventoryComponent::MergeItems(FItemRuntimeData& ItemData, int32 To)
{
	// �κ��丮�� ���� ������ + �κ��丮�� �ִ� ������ �� ��� 

	if (AddCount(To, ItemData.Count) == true)
	{
		// ���� ���� 
		return true;
	}
	else
	{
		return false;
	}
	
}

bool UMyInventoryComponent::MergeItems(int32 From, int32 To)
{
	// �κ��丮�� �ִ� ������ ���� ���� 
	if (AddCount(To, m_Inventory[From].Count) == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool UMyInventoryComponent::SlitItems(int32 From, int32 To)
{
	// (����) ��� ������ �ɰ������� 
	//			�ɰ��� ��ġ�� �ΰ��� ��� �������� 
	//			����� �� �غ����� �� ����. 


	return false;
}

void UMyInventoryComponent::SortInventory()
{
	// �����ۿ� ������ ������ ȣ���ϵ��� ���� .
	// ID��ȣ�� �� ���� ������� ����
	m_Inventory.Sort([](FInventoryData A, FInventoryData B) {return A.ItemStaticData.ID < B.ItemStaticData.ID; });
	
	// ���� �� ���� ������Ʈ.
	m_UpdateWidget.Broadcast();
}

void UMyInventoryComponent::SwapInventory(int32 From, int32 To)
{
	// ����Ʈ���� ��ü�� ����
	m_Inventory.Swap(From, To);

	// ���� �۾� (�� ��ü�� ���� Updateȣ�� )
	m_SwapWidget.Execute(From, To);
}

bool UMyInventoryComponent::Equip(int32 FromInven, int32 ToEquip)
{
	// ���ڵ��� -1�̸� ������ �ȵǾ��ٴ� ���̴�. 
	// FromInven (-1) : �κ��丮���� ��ҿ��� ���ȴ�. 
	// ToEquip (-1) : ��� ��ȣ�� ã�Ƽ� ����Ѵ�.
	if (FromInven <= NullIndex && ToEquip <= NullIndex) // �Ѵ� ������ �߸��� �Է�.
		return false;

	if (FromInven == NullIndex)
	{
		// �κ��丮�� ������ �ȵǸ�, �ʱ�ȭ�� �Ŷ�� ���� [ �ٸ� ��찡 ����� �����ؾ� �Ѵ�. ]
		return InitEquipment(ToEquip);
	}

	if (ToEquip == NullIndex)
	{
		// ����� ��Ʈ ��ȣ�� �޾Ƽ� �̵��� ��ġ�� Ư���Ѵ�.
		ToEquip = static_cast<int32>(m_Inventory[FromInven].ItemStaticData.EquipmentPart);
	}

	if (m_Inventory[FromInven].ItemStaticData.Type != static_cast<int32>(EItemType::EquipmentItem) 
		|| m_Inventory[FromInven].ItemStaticData.EquipmentPart != static_cast<EEquipmentPart>(ToEquip))
	{
		// ��� �ƴϰų�, ���� ������ Ÿ�� ����ġ   ����
		return false;
	}
	else
	{
		if (m_Inventory[ToEquip].ItemStaticData.Type == static_cast<int32>(EItemType::EquipmentItem))
		{

			// ĳ���Ϳ� ��� ���� 
			IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
			InventoryInterface->UnEquip(ToEquip);

			// ���� ��ȯ 
			FInventoryData Temp = m_Inventory[ToEquip];
			m_Inventory[ToEquip] = m_Inventory[FromInven];
			m_Inventory[FromInven] = Temp;

			// ��� ���� �� ���� ����
			m_Inventory[ToEquip].IsEquip = true;
			m_Inventory[FromInven].IsEquip = false;

			// ĳ���Ϳ� ��� ����
			InventoryInterface->Equip(ToEquip);
		}
		else
		{
			// ��� ���� �Ǿ� ���� ���� ��.
			m_Inventory[ToEquip] = m_Inventory[FromInven];
			m_Inventory[FromInven] = FInventoryData{};
			
			// ��� ���� ����
			m_Inventory[ToEquip].IsEquip = true;

			// ĳ���Ϳ� ��� ���� 
			IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
			InventoryInterface->Equip(ToEquip);

		}
		// ���� ����
		m_SwapWidget.Execute(FromInven, ToEquip);

		return true;
	}

}

bool UMyInventoryComponent::UnEquip(int32 FromEquip, int32 ToInven)
{
	// ���� NullIndex ���� �ϼ� ���� , �κ��丮�� NullIndex �̸� �ϼ� ����.
		if (FromEquip <= NullIndex || ToInven < NullIndex) 
		return false;

	// ����� ã�� ����  [ 
	if (ToInven == NullIndex)
	{
		for (ToInven = m_ItemFirstIndex; ToInven < m_Inventory.Num(); ToInven++)
		{
			if (m_Inventory[ToInven].ItemStaticData.Type == static_cast<int32>(EItemType::NullItem))
				break;
		}

		if (ToInven == m_Inventory.Num())
		{
			// �κ��丮�� ���� �����̴�. 
			return false;
		}
	}
	
	// ��� ���� ���� 
	if (m_Inventory[ToInven].ItemStaticData.Type == static_cast<int32>(EItemType::NullItem))
	{
		// ���� �Ǿ������鼭 ����� ��� �˻�.
		if (m_Inventory[FromEquip].IsEquip == true &&
			m_Inventory[FromEquip].ItemStaticData.Type == static_cast<int32>(EItemType::EquipmentItem))
		{
			
			// ĳ���Ϳ� ��� ����			// �κ��丮���� ���� ���� �Ǿ�� ���ȵ� ����
			IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
			InventoryInterface->UnEquip(FromEquip);

			// ���� ��ȯ
			m_Inventory[ToInven] = m_Inventory[FromEquip];
			m_Inventory[FromEquip] = FInventoryData{};

			// ��� ���� ����
			m_Inventory[ToInven].IsEquip = false;

			// ���� ���� 
			m_SwapWidget.Execute(FromEquip, ToInven);

			return true;
		}

	}
	return false;

}

bool UMyInventoryComponent::AddCount(int32 Index, int32 Count)
{
	int32 NewCount = m_Inventory[Index].Count + Count;
	if (NewCount > m_Inventory[Index].ItemStaticData.MaxCount)
	{
		// ������ �������� ������ �ʰ��ϸ� ��ĥ �� ����.

		return false;
	}
	else if (NewCount < 0)
	{
		// ������ �پ�� �� �� ����.
		return false;
	}
	else
	{
		m_Inventory[Index].Count = NewCount;

		// ���� ���� ����
		m_UpdateSlotWidget.Broadcast(Index);
		return true;
	}
}

bool UMyInventoryComponent::InitEquipments()
{
	// �ʱ� ��� ������ ���ִ� �Լ� 
	bool Result = true;
	for (int32 i = 0; i < m_ItemFirstIndex; i++)
	{
		// �ϳ��� �����ϸ� ���� ! 
		Result = (Result && InitEquipment(i));
	}
	return Result;
}

bool UMyInventoryComponent::InitEquipment(int32 Index)
{
	if (m_Inventory[Index].ItemStaticData.Type == static_cast<int32>(EItemType::EquipmentItem))
	{
		// ĳ���Ϳ� ��� ���� 
		IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
		InventoryInterface->Equip(Index);
		// ���� ����
		m_UpdateSlotWidget.Broadcast(Index);
		return true;
	}

	return false;
}

int32 UMyInventoryComponent::SearchPedingSlot(int32 ID , int32 Amount)
{
	for (int Index = m_ItemFirstIndex; Index < m_Inventory.Num(); Index++)
	{
		if (m_Inventory[Index].Count == 0)
		{
			// 2. ù �����    
			// �������� �ִٸ� ���ĵǾ nullptr�� �ȳ��´�. ������ nullptr���� ���´�.
			return Index;
		}
		else if (m_Inventory[Index].ItemStaticData.ID != ID)
		{
			continue;
		}
		else if (m_Inventory[Index].ItemStaticData.ID == ID)
		{
			// 1. ID�� ��ġ�ϴ� �����߿��� ã�� 
			
			// ������ ������ �߰��� ������ �ִ� ������ ���� �ʴ��� Ȯ��
			if (m_Inventory[Index].Count + Amount <= m_Inventory[Index].ItemStaticData.MaxCount)
			{
				return Index;
			}
			else
			{
				continue;
			}
		}
		
	}

	// ����������� �������� �� �ڸ��� ���ٴ� �ǹ�.
	return -1;
}
