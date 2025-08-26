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

			// 장비 데이터를 받아오는 부분 
			// 주소의 크기를(int32*) 통해서 배열처럼 쓰자.		
			// 단 이렇게 하면 확실하게 파트크기랑 데이터안에 ItemID데이터 구간 크기가 일치해야한다.
			// (임시) const로 값을 받으면서 ... const_cast로 캐스팅해서 사용하는데 문제가 없을까 ?
			int32* pItemID = const_cast<int32*>(& EquipmentData->HItemID);
			for (int32 Index = 0; Index < static_cast<int32>(EEquipmentPart::End); Index++,pItemID++)
			{
				if (*pItemID == -1) continue;

				auto ItemData = GameSubsystem->GetItemData(*pItemID);

				// 캐릭터에 장비 장착 
				// 캐릭터를 쓸려면 월드 생성이후 BeginPlay이후에 SetupData함수가 호출되어야한다.
				IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
				InventoryInterface->Equip(Index);


				if (InitEquipment(Index) == false)
				{
					// Debug
					UE_LOG(LogTemp, Log, TEXT(" %d Index Fail "), Index);
				}

			}

		}

		// 여기는 인벤토리를 데이터로 부터 받아올 부분


	}

}

/*
void UMyInventoryComponent::BindEquipment(TArray<USkeletalMeshComponent*> EquipmentList)
{
	// InitializeComponent 이후에 호출 되어야 유효하다. [ GetMesh가 유효한 시점에서 ] 

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
	// 인벤토리의 컨테이너는 여기서 초기화 !!!

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
	// 반환 값은 아이템 타입.
	uint8 ItemType = static_cast<uint8>(m_Inventory[Index].ItemStaticData.Type);
	// 소모 템 일때만 사용가능 
	if (ItemType == static_cast<uint8>(EItemType::ConsumableItem))
	{
		OutStatus = m_Inventory[Index].ItemStaticData.Status;
		if (m_Inventory[Index].Count >= 1)
		{
			// 아이템 내부에서 0이되면 제거하도록 되어있다.
			AddCount(Index,-1);
			// Update Widget
			m_UpdateSlotWidget.Broadcast(Index);
		}
		else
		{
			//  0보다 작으면 문제가 있다. // 0이면 아무일도 없어야한다. 
			return ItemType;
		}
		
	}
	
	return ItemType;
}

bool UMyInventoryComponent::UseEquipment(FBaseStatusData& OutStatus, int32 Index)
{
	// 반환 값은 아이템 타입.
		uint8 ItemType = static_cast<uint8>(m_Inventory[Index].ItemStaticData.Type);
	// 장비 템 일때만 사용가능 
	if (ItemType == static_cast<uint8>(EItemType::EquipmentItem))
	{
		if (m_Inventory[Index].Count == 1)
		{
			OutStatus = m_Inventory[Index].ItemStaticData.Status;
			return true;
		}
		else
		{
			//  1개가 아니면 문제가 있다. 
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
	// 강제로 특정 슬롯에 아이템을 밀어 넣을때 사용할 예정
	if (m_Inventory[Index].Count == 0)
	{

		// 아이템 추가
		m_Inventory[Index].ItemStaticData = ItemData.ItemStaticData;
		m_Inventory[Index].Count = ItemData.Count;

		// 위젯 슬롯 갱신
		m_UpdateSlotWidget.Broadcast(Index);

		return true;
	}
	else
	{
		// ID 확인 후 다르면 실패.
		if (m_Inventory[Index].ItemStaticData.ID == ItemData.ItemStaticData.ID)
		{
			// 아이템 병합
			return MergeItems(ItemData, Index);
		}

	}
	return false;

}

bool UMyInventoryComponent::TakeItem(FItemRuntimeData& ItemData)
{
	int32 Index = SearchPedingSlot(ItemData.ItemStaticData.ID, ItemData.Count);
	// 공간이 없을 때
	if (Index == -1) { return false; }


	if (m_Inventory[Index].Count == 0)
	{
		// 아이템 추가
		m_Inventory[Index].ItemStaticData = ItemData.ItemStaticData;
		m_Inventory[Index].Count = ItemData.Count;

		// 위젯 슬롯 갱신
		m_UpdateSlotWidget.Broadcast(Index);
		return true;
	}
	else
	{
		// 아이템 병합
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

		// 초기화 지만... 구조체의 초기화 값을 정해주지 않아서 그냥 0으로 초기화될 것이다. 
		m_Inventory[Index] = FInventoryData{};

		// 위젯 슬롯 갱신
		m_UpdateSlotWidget.Broadcast(Index);

		// 반환 되는 값은 아이템 데이터 값
		return RuntimeData;
	}
}

FItemRuntimeData UMyInventoryComponent::DropItem(int32 Index)
{
	// 여기서 뭘 해줘야할까 단순히 아이템을 Pop하는게 아니라 드롭하는 기능이라고 했을때 ? 
	// 만약에 차이점이 없으면 없애도 되는 함수기는 하다.
	return PopItem(Index);

}

bool UMyInventoryComponent::MergeItems(FItemRuntimeData& ItemData, int32 To)
{
	// 인벤토리에 없는 아이템 + 인벤토리에 있는 아이템 일 경우 

	if (AddCount(To, ItemData.Count) == true)
	{
		// 삭제 성공 
		return true;
	}
	else
	{
		return false;
	}
	
}

bool UMyInventoryComponent::MergeItems(int32 From, int32 To)
{
	// 인벤토리에 있는 아이템 끼리 병합 
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
	// (보류) 몇대 몇으로 쪼개야할지 
	//			쪼개진 위치를 두곳을 어디를 지정할지 
	//			고민을 좀 해봐야할 것 같다. 


	return false;
}

void UMyInventoryComponent::SortInventory()
{
	// 아이템에 변동이 있을때 호출하도록 하자 .
	// ID번호가 더 작은 순서대로 정렬
	m_Inventory.Sort([](FInventoryData A, FInventoryData B) {return A.ItemStaticData.ID < B.ItemStaticData.ID; });
	
	// 정렬 후 위젯 업데이트.
	m_UpdateWidget.Broadcast();
}

void UMyInventoryComponent::SwapInventory(int32 From, int32 To)
{
	// 리스트에서 객체를 세팅
	m_Inventory.Swap(From, To);

	// 위젯 작업 (각 객체의 위젯 Update호출 )
	m_SwapWidget.Execute(From, To);
}

bool UMyInventoryComponent::Equip(int32 FromInven, int32 ToEquip)
{
	// 인자들이 -1이면 지정이 안되었다는 뜻이다. 
	// FromInven (-1) : 인벤토리외의 장소에서 장비된다. 
	// ToEquip (-1) : 장비 번호를 찾아서 장비한다.
	if (FromInven <= NullIndex && ToEquip <= NullIndex) // 둘다 음수면 잘못된 입력.
		return false;

	if (FromInven == NullIndex)
	{
		// 인벤토리가 지정이 안되면, 초기화일 거라고 추정 [ 다른 경우가 생기면 수정해야 한다. ]
		return InitEquipment(ToEquip);
	}

	if (ToEquip == NullIndex)
	{
		// 장비의 파트 번호를 받아서 이동할 위치를 특정한다.
		ToEquip = static_cast<int32>(m_Inventory[FromInven].ItemStaticData.EquipmentPart);
	}

	if (m_Inventory[FromInven].ItemStaticData.Type != static_cast<int32>(EItemType::EquipmentItem) 
		|| m_Inventory[FromInven].ItemStaticData.EquipmentPart != static_cast<EEquipmentPart>(ToEquip))
	{
		// 장비가 아니거나, 장비와 슬롯이 타입 불일치   실패
		return false;
	}
	else
	{
		if (m_Inventory[ToEquip].ItemStaticData.Type == static_cast<int32>(EItemType::EquipmentItem))
		{

			// 캐릭터에 장비 해제 
			IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
			InventoryInterface->UnEquip(ToEquip);

			// 슬롯 교환 
			FInventoryData Temp = m_Inventory[ToEquip];
			m_Inventory[ToEquip] = m_Inventory[FromInven];
			m_Inventory[FromInven] = Temp;

			// 장비 해제 및 장착 설정
			m_Inventory[ToEquip].IsEquip = true;
			m_Inventory[FromInven].IsEquip = false;

			// 캐릭터에 장비 장착
			InventoryInterface->Equip(ToEquip);
		}
		else
		{
			// 장비가 장착 되어 있지 않을 때.
			m_Inventory[ToEquip] = m_Inventory[FromInven];
			m_Inventory[FromInven] = FInventoryData{};
			
			// 장비 장착 설정
			m_Inventory[ToEquip].IsEquip = true;

			// 캐릭터에 장비 장착 
			IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
			InventoryInterface->Equip(ToEquip);

		}
		// 위젯 갱신
		m_SwapWidget.Execute(FromInven, ToEquip);

		return true;
	}

}

bool UMyInventoryComponent::UnEquip(int32 FromEquip, int32 ToInven)
{
	// 장비는 NullIndex 이하 일수 없고 , 인벤토리는 NullIndex 미만 일수 없다.
		if (FromEquip <= NullIndex || ToInven < NullIndex) 
		return false;

	// 빈공간 찾는 로직  [ 
	if (ToInven == NullIndex)
	{
		for (ToInven = m_ItemFirstIndex; ToInven < m_Inventory.Num(); ToInven++)
		{
			if (m_Inventory[ToInven].ItemStaticData.Type == static_cast<int32>(EItemType::NullItem))
				break;
		}

		if (ToInven == m_Inventory.Num())
		{
			// 인벤토리가 가득 찬것이다. 
			return false;
		}
	}
	
	// 장비 해제 로직 
	if (m_Inventory[ToInven].ItemStaticData.Type == static_cast<int32>(EItemType::NullItem))
	{
		// 장착 되어있으면서 장비인 경우 검사.
		if (m_Inventory[FromEquip].IsEquip == true &&
			m_Inventory[FromEquip].ItemStaticData.Type == static_cast<int32>(EItemType::EquipmentItem))
		{
			
			// 캐릭터에 장비 제거			// 인벤토리보다 먼저 적용 되어야 스탯도 적용
			IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
			InventoryInterface->UnEquip(FromEquip);

			// 슬롯 교환
			m_Inventory[ToInven] = m_Inventory[FromEquip];
			m_Inventory[FromEquip] = FInventoryData{};

			// 장비 설정 해제
			m_Inventory[ToInven].IsEquip = false;

			// 위젯 갱신 
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
		// 갯수를 합했을때 갯수가 초과하면 합칠 수 없다.

		return false;
	}
	else if (NewCount < 0)
	{
		// 음수로 줄어들어도 뺄 수 없다.
		return false;
	}
	else
	{
		m_Inventory[Index].Count = NewCount;

		// 위젯 슬롯 갱신
		m_UpdateSlotWidget.Broadcast(Index);
		return true;
	}
}

bool UMyInventoryComponent::InitEquipments()
{
	// 초기 장비 설정을 해주는 함수 
	bool Result = true;
	for (int32 i = 0; i < m_ItemFirstIndex; i++)
	{
		// 하나라도 실패하면 실패 ! 
		Result = (Result && InitEquipment(i));
	}
	return Result;
}

bool UMyInventoryComponent::InitEquipment(int32 Index)
{
	if (m_Inventory[Index].ItemStaticData.Type == static_cast<int32>(EItemType::EquipmentItem))
	{
		// 캐릭터에 장비 장착 
		IInventoryInterface* InventoryInterface = CastChecked<IInventoryInterface>(GetOwner());
		InventoryInterface->Equip(Index);
		// 위젯 갱신
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
			// 2. 첫 빈공간    
			// 아이템이 있다면 정렬되어서 nullptr이 안나온다. 없으면 nullptr부터 나온다.
			return Index;
		}
		else if (m_Inventory[Index].ItemStaticData.ID != ID)
		{
			continue;
		}
		else if (m_Inventory[Index].ItemStaticData.ID == ID)
		{
			// 1. ID가 일치하는 공간중에서 찾기 
			
			// 기존의 공간과 추가할 공간이 최대 공간을 넘지 않는지 확인
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

	// 여기까지오면 아이템이 들어갈 자리가 없다는 의미.
	return -1;
}
