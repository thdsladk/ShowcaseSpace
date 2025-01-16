// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInventoryWidget.generated.h"



/**
 * 
 */
UCLASS()
class MYTEST_TOPDOWN_API UMyInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	virtual void NativeConstruct() override;


	UFUNCTION()
	void UpdateWidget();
	UFUNCTION()
	void UpdateWidget_Slot(int32 Index);
	UFUNCTION()
	void BindInventoryComp(class UMyInventoryComponent* InventoryComp);
	FORCEINLINE
	void Swap_ItemSlot(int32 From, int32 To) { UpdateWidget_Slot(From); UpdateWidget_Slot(To); }

protected:

	UPROPERTY()
	TArray<class USlotWidget*> m_ItemSlotList;
	UPROPERTY()
	TArray<class USlotWidget*> m_EquipSlotList;
	

	//UPROPERTY()
	//TWeakObjectPtr<class UMyInventoryComponent> m_InventoryComp;



	const uint32 m_Row = 5U;
	const uint32 m_Column = 8U;

};
