// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterComponent/MyEquipmentComponent.h"

#include "GameFramework/Character.h"

#include "CharacterComponent/MyInventoryComponent.h"


// Sets default values for this component's properties
UMyEquipmentComponent::UMyEquipmentComponent()
	:m_CharacterMesh(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	//m_PartTypeList.Init(TEXT(""), static_cast<int32>(EEquipmentPart::End));
	//m_PartTypeList[static_cast<int32>(EEquipmentPart::Head)]	=	TEXT("Head_Socket");
	//m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_L)]	=	TEXT("LeftHand_Socket");
	//m_PartTypeList[static_cast<int32>(EEquipmentPart::Chest)]	=	TEXT("Chest_Socket");
	//m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_R)]	=	TEXT("RightHand_Socket");
	//m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_L)]	=	TEXT("LeftFoot_Socket");
	//m_PartTypeList[static_cast<int32>(EEquipmentPart::Foot_R)]	=	TEXT("RightFoot_Socket");
	//
	//m_Equipment.Init(nullptr, static_cast<int32>(EEquipmentPart::End));
	//
	//m_EquipmentIndex.Init(-1, static_cast<int32>(EEquipmentPart::End));

}

void UMyEquipmentComponent::InitializeComponent()
{
	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	m_CharacterMesh = Owner->GetMesh();
	
	// 컨트롤러 가져오기.
	//AMyPlayerController* PC = CastChecked<AMyPlayerController>(Owner->GetController());

	
	
	
	//m_Equipment[static_cast<int32>(EEquipmentPart::Hand_L)] = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());
	//
	//if (m_Equipment[static_cast<int32>(EEquipmentPart::Hand_L)] != nullptr)
	//{
	//	m_Equipment[static_cast<int32>(EEquipmentPart::Hand_L)]->SetItem(LItemID);
	//	m_Equipment[static_cast<int32>(EEquipmentPart::Hand_L)]->Equip();
	//
	//	if (m_Equipment[static_cast<int32>(EEquipmentPart::Hand_L)]->AttachToComponent(&Mesh,
	//		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//		m_PartTypeList[static_cast<int32>(EEquipmentPart::Hand_L)]) == false)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT(" Not Attached "));
	//	}
	//
	//	m_Equipment[static_cast<int32>(EEquipmentPart::Hand_L)]->FinishSpawning(FTransform());
	//}

}
