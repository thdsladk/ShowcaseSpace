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
	
	m_Inventory.Init(TArray<AMyItem*>(), (m_Row * m_Column) + 6U);//);  �ӽ� �� +6U.

	
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


		if (ItemType != EItemEffectType::None)	// None�̸� Usable �������� �ƴϴ�.
		{
			PopBackItem(Index);


			// ���� ... ���⼭ ���Ӹ��� �κ��丮 ������ ĳ�����ؼ� �ᵵ �Ǵ°� ??? [ ������ ���� ������Ʈ�� ���ؼ� ]
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
		// Widget Update  : ���⼭ ���� ���������� �ƴϸ� Index�� �޾Ƽ� �ش� �ش� ���Ը� ���������� ���ϸ� ������
		//					�ӽ÷� �κ��丮 ������ �����Ѵ�.
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

	// ����������� ���и� ��ȯ�ؾ��Ѵ�.
	return -1;
}

void UMyInventoryComponent::SwapInventory(int32 From, int32 To)
{
	// ����Ʈ���� ��ü�� ����
	m_Inventory.Swap(From, To);

	// ���� �۾� (�� ��ü�� ���� Updateȣ�� )
	m_SwapWidget.Execute(From, To);
}

int32 UMyInventoryComponent::GetInventorySize()
{
	return (m_Row*m_Column);
}
