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
        // ������ ������ ���� �� �ִ�.
    }
    else if(Index < static_cast<int32>(EEquipmentPart::End))
    {
        m_EquipSlotList[Index]->UpdateWidget();
    }
    else    // ���â ������ ������â
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
    // �κ��丮 ������Ʈ�� �޾Ƽ� ���⼭ ���ε�������.
    if (IsValid(&InventoryComp))
    {
        // ���ε� InventoryComponent �� InventoryWidget�� UpdateWidget�Լ��� ���ε�.
        InventoryComp.m_UpdateWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget);
        InventoryComp.m_UpdateSlotWidget.AddUObject(this, &UMyInventoryWidget::UpdateWidget_Slot);
        InventoryComp.m_SwapWidget.BindUObject(this, &UMyInventoryWidget::Swap_ItemSlot);
        
        // �ʱ� ���� 

        FString WidgetNamePattern = TEXT("EquipmentSlot_"); // �̹��� ���� �̸��� ����
        for (uint32 Index = 0U; Index < 6U; Index++)
        {
            // EquipSlot ������ �̸��� ����
            const FString EquipSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index);

            // EquipSlot ������ ���ε�
            m_EquipSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*EquipSlotWidgetName));

            // EquipSlot ���� �ʱ�ȭ
            m_EquipSlotList[Index]->BindInventory(&InventoryComp);

            // EquipSlot Setting        
            m_EquipSlotList[Index]->SetSlotIndex(Index);
            m_EquipSlotList[Index]->SetSlotType(ESlotType::EquipmentSlot);

            // ����
            m_EquipSlotList[Index]->UpdateWidget();
        }
        
        
        WidgetNamePattern = TEXT("ItemSlot_"); // �̹��� ���� �̸��� ����
        for (uint32 Index = 0U; Index < (InvenSize); Index++)
        {
            uint32 Index_Oct = 0U;
            Index_Oct += (Index / Column) * 10U;
            Index_Oct += (Index % Column);

            // ItemSlot ������ �̸��� ����
            const FString ItemSlotWidgetName = WidgetNamePattern + FString::Printf(TEXT("%02d"), Index_Oct);

            // ItemSlot ������ ���ε�
            m_ItemSlotList[Index] = Cast<USlotWidget>(GetWidgetFromName(*ItemSlotWidgetName));

            // ItemSlot ���� �ʱ�ȭ
            m_ItemSlotList[Index]->BindInventory(&InventoryComp);

            // ItemSlot Setting
            m_ItemSlotList[Index]->SetSlotIndex(Index +static_cast<int32>(EEquipmentPart::End));

            // ����
            m_ItemSlotList[Index]->UpdateWidget();
        }

    
    }
    
}






