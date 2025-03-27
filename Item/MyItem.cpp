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
			
			// 스테틱 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
			m_ItemInfo.ItemMesh = LoadObject<UStaticMesh>(NULL, *ItemData->ItemMesh->GetPathName() , NULL, LOAD_None, NULL);
			// 스테틱 메쉬 생성과정 [ 객체를 직접 복사해서 붙여 주는중 ]
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
				m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.Scale);	// 스케일 두배로.
			}
			// 설명서 추가하기 . [ 아이템이 다 세팅된 다음에 해야한다.
			m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "),*m_ItemInfo.Name);

		}

	}
}

void AMyItem::Init()
{
	// 부모가 가지고 있는 멤버들 세팅
	Super::Init();
	// 자식이 가지고 있는 멤버들 세팅


}

void AMyItem::Click_F()
{
}

void AMyItem::Interact_Implementation()
{
	// 땅에 떨어지면 줍기 , 아니면 다른 상호작용을 실행하도록 한다 .
	// bool 보단 상태머신으로 만드는것도 좋을듯하다.

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
	// OFF 상태로 숨겨주기 . 
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
	// 부모가 가지고 있는 멤버들 세팅
	Super::Replace(Pos);
	// 자식이 가지고 있는 멤버들 세팅
	m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.Scale);	// 스케일 두배로.
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


