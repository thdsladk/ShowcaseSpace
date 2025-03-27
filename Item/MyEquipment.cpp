// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEquipment.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EditorFramework/AssetImportData.h"
#include "Components/WidgetComponent.h"

#include "ItemHelpTip.h"


// Sets default values
AMyEquipment::AMyEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AMyEquipment::SetEquipment(int32 NumEquipment)
{

}

void AMyEquipment::SetItem(int32 ID)
{
	Super::SetItem(ID);

	SetEquipment(ID);

}

void AMyEquipment::OnEquip()
{
	if (m_IsEquip == false)
	{
		m_IsEquip = true;
		SwitchVisiable();
	}

}

void AMyEquipment::DetachEquip()
{
	if (m_IsEquip == true)
	{
		m_IsEquip = false;
		SwitchVisiable();
	}
}

void AMyEquipment::Click_F()
{
}

void AMyEquipment::Interact_Implementation()
{
	// ���� �������� �ݱ� , �ƴϸ� �ٸ� ��ȣ�ۿ��� �����ϵ��� �Ѵ� .
// bool ���� ���¸ӽ����� ����°͵� �������ϴ�.

	if (m_IsGround)
	{
		OnPickedUp();
	}
	else
	{
		Click_F();
	}
}

void AMyEquipment::OnPickedUp()
{
	Super::OnPickedUp();
}

// Called when the game starts or when spawned
void AMyEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyEquipment::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCharacterBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMyEquipment::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCharacterEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}

