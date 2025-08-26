// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInventoryWidget.generated.h"


class USlotWidget;
class UMyInventoryComponent;
/**
 * 
 */



UCLASS()
class MYTEST_TOPDOWN_API UMyInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	void BindInventory(UMyInventoryComponent& InventoryComp);

	UFUNCTION()
	void UpdateWidget();
	UFUNCTION()
	void UpdateWidget_Slot(int32 Index);
	FORCEINLINE
	void Swap_ItemSlot(int32 From, int32 To) { UpdateWidget_Slot(From); UpdateWidget_Slot(To); }

	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);


protected:

	UPROPERTY()
	TArray<USlotWidget*> m_EquipSlotList;
	UPROPERTY()
	TArray<USlotWidget*> m_ItemSlotList;
	




};
