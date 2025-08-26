// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInventoryWidget.h"
#include "SlotWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"
#include "Blueprint/DragDropOperation.h"

#include "CharacterComponent/MyInventoryComponent.h"

#include "Header/GlobalEnum.h"

constexpr uint32 Row = 5U;
constexpr uint32 Column = 8U;
constexpr uint32 InvenSize = 40U;

void UMyInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    m_EquipSlotList.Init(nullptr, static_cast<uint32>(EEquipmentPart::End));
    m_ItemSlotList.Init(nullptr, (InvenSize));
    
}

void UMyInventoryWidget::UpdateWidget()
{
    for (auto ItemSlot:m_ItemSlotList)
        ItemSlot->UpdateWidget();

    for (auto EquipmentSlot : m_EquipSlotList)
        EquipmentSlot->UpdateWidget();

}

void UMyInventoryWidget::UpdateWidget_Slot(int32 Index)
{
    if (Index < 0)
    {
        // 음수면 문제가 있을 수 있다.
    }
    else if(Index < static_cast<int32>(EEquipmentPart::End))
    {
        m_EquipSlotList[Index]->UpdateWidget();
    }
    else    // 장비창 다음은 아이템창
    {
        m_ItemSlotList[Index - static_cast<int32>(EEquipmentPart::End)]->UpdateWidget();
    }
}

void UMyInventoryWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    InOperation->Tag = TEXT("EnterWidget");
}

void UMyInventoryWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    InOperation->Tag = TEXT("LeaveWidget");
}

void UMyInventoryWidget::BindInventory(UMyInventoryComponent& InventoryComp)
{
    // 인벤토리 컴포넌트를 받아서 여기서 바인딩해주자.
    if (IsValid(&InventoryComp))
    {
        // 바인딩 InventoryComponent 와 InventoryWidget의 UpdateWidget함수를 바인딩.
        InventoryComp.m_UpdateWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget);
        InventoryComp.m_UpdateSlotWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget_Slot);
        InventoryComp.m_SwapWidget.BindUObject(this, &UMyInventoryWidget::Swap_ItemSlot);
        
        // 초기 세팅 

        FString WidgetNamePattern = TEXT("EquipmentSlot_"); // 이미지 위젯 이름의 패턴
        for (uint32 Index = 0U; Index < 6U; Index++)
        {
            // EquipSlot 위젯의 이름을 생성
            const FString EquipSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index);

            // EquipSlot 위젯을 바인딩
            m_EquipSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*EquipSlotWidgetName));

            // EquipSlot 위젯 초기화
            m_EquipSlotList[Index]->BindInventory(&InventoryComp);

            // EquipSlot Setting        
            m_EquipSlotList[Index]->SetSlotIndex(Index);
            m_EquipSlotList[Index]->SetSlotType(ESlotType::EquipmentSlot);

            // 갱신
            m_EquipSlotList[Index]->UpdateWidget();
        }
        
        
        WidgetNamePattern = TEXT("ItemSlot_"); // 이미지 위젯 이름의 패턴
        for (uint32 Index = 0U; Index < (InvenSize); Index++)
        {
            uint32 Index_Oct = 0U;
            Index_Oct += (Index / Column) * 10U;
            Index_Oct += (Index % Column);

            // ItemSlot 위젯의 이름을 생성
            const FString ItemSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index_Oct);

            // ItemSlot 위젯을 바인딩
            m_ItemSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*ItemSlotWidgetName));

            // ItemSlot 위젯 초기화
            m_ItemSlotList[Index]->BindInventory(&InventoryComp);

            // ItemSlot Setting
            m_ItemSlotList[Index]->SetSlotIndex(Index +static_cast<int32>(EEquipmentPart::End));

            // 갱신
            m_ItemSlotList[Index]->UpdateWidget();
        }

    
    }
    
}






