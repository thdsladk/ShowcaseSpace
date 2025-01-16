// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventoryComponent.h"
#include "MyItem.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyTest_TopDownGameMode.h"
#include "MyInventoryWidget.h"


// Sets default values for this component's properties
UMyInventoryComponent::UMyInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	m_Inventory.Init(TArray<AMyItem*>(), (m_Row * m_Column) + 6U);//);  임시 로 +6U.

	
}

// Called when the game starts
void UMyInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

UTexture2D* UMyInventoryComponent::GetThumbnail(int32 Index)
{
	if (m_Inventory[Index].IsEmpty())
		return nullptr;
	return m_Inventory[Index].Top()->GetThumbnail();
}

int32 UMyInventoryComponent::GetItemCount(int32 Index)
{
	if (m_Inventory[Index].IsEmpty())
		return NULL;
	return m_Inventory[Index].Num();
}

FString UMyInventoryComponent::GetItemName(int32 Index)
{
	if (m_Inventory[Index].IsEmpty())
		return FString(TEXT("Empty"));
	return m_Inventory[Index].Top()->GetName();
}

bool UMyInventoryComponent::PushBackItem(AMyItem* Item, int32 Index)
{
	if (m_Inventory[Index].IsEmpty() == true)
	{
		m_Inventory[Index].Push(Item);
		return true;
	}
	else
	{
		if (m_Inventory[Index].Num() < Item->GetMaxCount()
			&& m_Inventory[Index].Top()->GetID() == Item->GetID())
		{
			m_Inventory[Index].Push(Item);
			return true;
		}

	}
	return false;

}

AMyItem* UMyInventoryComponent::PopBackItem(int32 Index)
{
	if (m_Inventory[Index].IsEmpty() == true)
	{
		return nullptr;
	}
	else
	{
		return m_Inventory[Index].Pop();
	}
}

TArray<int32> UMyInventoryComponent::UseItem(int32 Index)
{
	if (m_Inventory[Index].IsEmpty() == false)
	{
		int32 ItemType = m_Inventory[Index].Last()->UsingItem();
		int32 ItemTime = m_Inventory[Index].Last()->GetEffectTime();
		int32 ItemIntensity = m_Inventory[Index].Last()->GetEffectIntensity();


		if (ItemType != EItemEffectType::None)	// None이면 Usable 아이템이 아니다.
		{
			PopBackItem(Index);


			// 주의 ... 여기서 게임모드랑 인벤토리 위젯을 캐스팅해서 써도 되는가 ??? [ 오로지 위젯 업데이트를 위해서 ]
			AMyTest_TopDownGameMode* GameMode = Cast<AMyTest_TopDownGameMode>(GetWorld()->GetAuthGameMode());
			if (GameMode != nullptr)
			{
				UMyInventoryWidget* CurrentWidget = Cast<UMyInventoryWidget>(GameMode->GetCurrentWidget());
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->UpdateWidget_Slot(Index);
				}
			
			}
		}
		// Widget Update  : 여기서 전부 갱신해줄지 아니면 Index를 받아서 해당 해당 슬롯만 갱신해줄지 정하면 좋을듯
		//					임시로 인벤토리 위젯을 갱신한다.
		m_UpdateWidget.Broadcast();

		//ItemType
		return TArray<int32>{ItemType, ItemTime, ItemIntensity};

	}
	else
	{
		return TArray<int32>{};
	}
	
}

void UMyInventoryComponent::DropItem(int32 Index, FVector Pos)
{

	for (auto Elem : m_Inventory[Index])
	{
		Elem->Replace(Pos);
	}
	m_Inventory[Index].Empty();
}

int32 UMyInventoryComponent::FindAddIndex(int32 ID)
{
	for (int i =0; i<m_Inventory.Num();i++)
	{
		if (m_Inventory[i].IsEmpty() == true)
		{
			return i;
		}
		else if((m_Inventory[i].Top()->GetID() == ID)
			&& (m_Inventory[i].Num() < m_Inventory[i].Top()->GetMaxCount()))
		{
			return i;
		}
	}

	// 여기까지오면 실패를 반환해야한다.
	return -1;
}

void UMyInventoryComponent::SwapInventory(int32 From, int32 To)
{
	// 리스트에서 객체를 세팅
	m_Inventory.Swap(From, To);

	// 위젯 작업 (각 객체의 위젯 Update호출 )
	m_SwapWidget.Execute(From, To);
}

int32 UMyInventoryComponent::GetInventorySize()
{
	return (m_Row*m_Column);
}
