// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemHelpTip.h"

#include "MyTest_TopDownCharacter.h"
#include "NiagaraComponent.h"




// Sets default values
AMyItem::AMyItem()
	: m_IsGround(true) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	m_ItemInfo.ID = 0;
}

void AMyItem::SetItem(int32 ID)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance != nullptr)
	{
		auto ItemData = MyGameInstance->GetItemData(ID);
		if (ItemData != nullptr)
		{
			m_ItemInfo.ID = ItemData->ID;
			m_ItemInfo.Type = ItemData->Type;
			m_ItemInfo.Name = ItemData->Name;
			m_ItemInfo.Description = ItemData->Description;
			m_ItemInfo.MaxCount = ItemData->MaxCount;
			m_ItemInfo.Scale = ItemData->Scale;
			m_ItemInfo.EffectType = ItemData->EffectType;
			m_ItemInfo.EffectTime = ItemData->EffectTime;
			m_ItemInfo.EffectIntensity = ItemData->EffectIntensity;

			m_ItemInfo.Thumbnail = ItemData->Thumbnail;
			
			// ����ƽ �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
			m_ItemInfo.ItemMesh = LoadObject<UStaticMesh>(NULL, *ItemData->ItemMesh->GetPathName() , NULL, LOAD_None, NULL);
			// ����ƽ �޽� �������� [ ��ü�� ���� �����ؼ� �ٿ� �ִ��� ]
			//if (nullptr != ItemData->ItemMesh)
			//{
			//	DuplicateObject(ItemData->ItemMesh, m_ItemMesh);
			//}

			if (nullptr != m_ItemInfo.ItemMesh)
			{
				m_MeshComp->SetStaticMesh(m_ItemInfo.ItemMesh);
				//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
				m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
				m_MeshComp->SetSimulatePhysics(true);
				m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.Scale);	// ������ �ι��.
			}
			// ���� �߰��ϱ� . [ �������� �� ���õ� ������ �ؾ��Ѵ�.
			m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "),*m_ItemInfo.Name);

		}

	}
}

void AMyItem::Init()
{
	// �θ� ������ �ִ� ����� ����
	Super::Init();
	// �ڽ��� ������ �ִ� ����� ����


}

void AMyItem::Click_F()
{
}

void AMyItem::Interact_Implementation()
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

void AMyItem::OnPickedUp()
{
	// OFF ���·� �����ֱ� . 
	AMyInteractable::m_Effect->Activate(true);


	m_IsGround = false;

	m_bVisiable = false;

	m_HelpTextComp->SetVisibility(m_bVisiable);
	//m_HelpTextComp->SetActive(false);

	SetHidden(true);

	m_Trigger->SetSimulatePhysics(false);
	m_Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32  AMyItem::UsingItem()
{
	return m_ItemInfo.EffectType;
}

void AMyItem::Replace(const FVector Pos)
{
	// �θ� ������ �ִ� ����� ����
	Super::Replace(Pos);
	// �ڽ��� ������ �ִ� ����� ����
	m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.Scale);	// ������ �ι��.
	m_IsGround = true;
}

void AMyItem::SetHidden(bool bHide)
{
	m_MeshComp->SetVisibility(!bHide);
	m_MeshComp->SetSimulatePhysics(!bHide);
	m_MeshComp->SetHiddenInGame(bHide);
}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();

	
}

void AMyItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetItem(m_ItemInfo.ID);

	auto HelpWidget = Cast<UItemHelpTip>(m_HelpTextComp->GetUserWidgetObject());
	if (HelpWidget)
		HelpWidget->BindHelpTip(m_ItemInfo.Name, m_InteractableHelpText);

}

void AMyItem::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCharacterBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMyItem::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnCharacterEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
}


