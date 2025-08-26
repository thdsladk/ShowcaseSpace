// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/GlobalEnum.h"
#include "MyEquipmentComponent.generated.h"

class UMyInventoryWidget;
class AMyTest_TopDownPlayerController;
class UMyInventoryComponent;
class AMyEquipment;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTEST_TOPDOWN_API UMyEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyEquipmentComponent();

protected:
	virtual void InitializeComponent() override;

public:	


protected:

	TWeakObjectPtr<USkeletalMeshComponent> m_CharacterMesh;
	TWeakObjectPtr<UMyInventoryComponent> m_InventoryComp;

	/*
		���� �κ��丮 ������Ʈ���� ��� ���� ����� ���� �������̴�. 
		����δ� ��� ������Ʈ�� �ʿ䰡 ���»���.
	*/

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	//TArray<TWeakObjectPtr<AMyEquipment>> m_Equipment;


		
};
