// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */

UENUM()
enum class ESlotType : int32
{
	NoneSlot,
	ItemSlot,
	EquipmentSlot,
	SkillSlot,
	QuickSlot,
	QItemSlot,
	QSkillSlot
};


UCLASS()
class MYTEST_TOPDOWN_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void Init();


	UFUNCTION()
	void UpdateWidget();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;



	UFUNCTION()
	void SetThumbnail(class UTexture2D* Texture);
	void SetName(class FText TextBlock);
	void SetQuantity(class FText TextBlock);

	void SetEmpty();

	void SetSlotIndex(int32 Index) { m_SlotIndex = Index; }
	void SetSlotType(ESlotType Type) { m_SlotType = Type; }

protected:
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Image;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Quantity;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_SlotIndex =0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	ESlotType m_SlotType  = ESlotType::NoneSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlotWidget> m_EquipSlotClass;

	//Reference
	UPROPERTY()
	TWeakObjectPtr<class UMyInventoryComponent> m_InventoryComp;
};
