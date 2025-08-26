// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "Kismet/GameplayStatics.h"
#include "data/MyGameSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemHelpTip.h"

#include "NiagaraComponent.h"

// Sets default values
AMyItem::AMyItem()
	: m_IsGround(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	m_ItemInfo.ItemStaticData.ID = 0;
	m_ItemInfo.Count = 0;

	// 필요할때 생성해서 쓰자.
	//if (m_EquipmentMeshComp == nullptr)
	//{
	//	m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	//	m_EquipmentMeshComp->SetupAttachment(RootComponent);
	//}
}

void AMyItem::BeginPlay()
{
	Super::BeginPlay();

	// 월드에 생성될때 표시되도록 한다. 
	ApplyHidden(false);
}

void AMyItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto HelpWidget = Cast<UItemHelpTip>(m_HelpTextComp->GetUserWidgetObject());
	if (HelpWidget)
		HelpWidget->BindHelpTip(m_ItemInfo.ItemStaticData.Name, m_InteractableHelpText);
	
	InitItem();
}

void AMyItem::InitItem()
{
	if (m_DataTableRowHandle.IsNull() == true)
		return;


	FString ContextStr = GetName() + TEXT("_") + m_DataTableRowHandle.RowName.ToString();



	if (const FItemData* FoundRow = m_DataTableRowHandle.GetRow<FItemData>(ContextStr))
	{
		m_ItemInfo.ItemStaticData = *FoundRow;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row not found: %s"), *m_DataTableRowHandle.RowName.ToString());
	}

	// m_ItemInfo.Count 는 에디터에서 넣어주자. 

	switch (m_ItemInfo.ItemStaticData.Type)
	{
	case static_cast<int32>(EItemType::EquipmentItem):
	{
		// 스켈레탈 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
		if (m_ItemInfo.ItemStaticData.EquipmentMesh.IsValid() == false)
		{
			m_ItemInfo.ItemStaticData.EquipmentMesh.LoadSynchronous();
		}
		FString Path = m_ItemInfo.ItemStaticData.EquipmentMesh->GetPathName();
		USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *(Path), NULL, LOAD_None, NULL);

		if (SkeletalMesh != nullptr)
		{
			// 필요한 경우만 생성해서 쓰도록 
			if (m_EquipmentMeshComp == nullptr)
			{
				// 1. NewObject로 생성
				m_EquipmentMeshComp = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("EquipmentMesh_Runtime"));
				// 2. 액터에 컴포넌트 소유권 부여
				AddInstanceComponent(m_EquipmentMeshComp);
				// 3. 어태치 설정
				m_EquipmentMeshComp->SetupAttachment(RootComponent);
				// 4. 월드에 등록 (렌더/틱/물리 반영 시작)
				m_EquipmentMeshComp->RegisterComponent();
			}

			m_EquipmentMeshComp->SetSkeletalMesh(SkeletalMesh);
			m_EquipmentMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_EquipmentMeshComp->SetSimulatePhysics(true);
			m_EquipmentMeshComp->SetWorldScale3D(m_EquipmentMeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.

		}
		break;
	}
	case static_cast<int32>(EItemType::ConsumableItem):
	case static_cast<int32>(EItemType::Non_ConsumableItem):
	case static_cast<int32>(EItemType::EtcItem):
	{
		// 스테틱 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
		UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *m_ItemInfo.ItemStaticData.ItemMesh->GetPathName(), NULL, LOAD_None, NULL);

		if (StaticMesh != nullptr)
		{
			m_MeshComp->SetStaticMesh(StaticMesh);
			//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
			m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_MeshComp->SetSimulatePhysics(true);
			m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.
		}
		break;
	}
	default:
	{
		// NullItem, End 일때는 실패를 띄우자.
		return;
	}
	}

	// 설명서 추가하기 . [ 아이템이 다 세팅된 다음에 해야한다.
	m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "), *m_ItemInfo.ItemStaticData.Name);

}

void AMyItem::SetItem(int32 ID, uint8 Count)
{
	if(GetWorld() == nullptr)
		return;
	
	UMyGameSubsystem* GameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (GameSubsystem != nullptr)
	{
		auto ItemData = GameSubsystem->GetItemData(ID);
		if (ItemData != nullptr)
		{
			m_ItemInfo.Count = Count;
			m_ItemInfo.ItemStaticData = *ItemData;

			switch (m_ItemInfo.ItemStaticData.Type)
			{
			case static_cast<int32>(EItemType::EquipmentItem):
			{
				// 스켈레탈 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
				USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *ItemData->EquipmentMesh->GetPathName(), NULL, LOAD_None, NULL);


				if (SkeletalMesh != nullptr)
				{
					// (임시) 여기서 필요할때 장비메시 컴포넌트를 생성해줘보자.
					if (m_EquipmentMeshComp == nullptr)
					{
						m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
						m_EquipmentMeshComp->SetupAttachment(RootComponent);
					}

					m_EquipmentMeshComp->SetSkeletalMesh(SkeletalMesh);
					m_EquipmentMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
					m_EquipmentMeshComp->SetSimulatePhysics(true);
					m_EquipmentMeshComp->SetWorldScale3D(m_EquipmentMeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.
				}
				break;
			}
			case static_cast<int32>(EItemType::ConsumableItem):
			case static_cast<int32>(EItemType::Non_ConsumableItem):
			case static_cast<int32>(EItemType::EtcItem):
			{
				// 스테틱 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
				UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *ItemData->ItemMesh->GetPathName(), NULL, LOAD_None, NULL);

				if (StaticMesh != nullptr)
				{
					m_MeshComp->SetStaticMesh(StaticMesh);
					//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
					m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
					m_MeshComp->SetSimulatePhysics(true);
					m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.
				}
				break;
			}
			case static_cast<int32>(EItemType::NullItem):
			{
				break;
			}
			default:
			{
				break;
			}
			}

			// 설명서 추가하기 . [ 아이템이 다 세팅된 다음에 해야한다.
			m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "), *m_ItemInfo.ItemStaticData.Name);

		}
	}


}

bool AMyItem::InitData(FItemRuntimeData& RuntimeData)
{
	// 외부에서 아이템 데이터를 받아온다. 
	if (RuntimeData.Count <= 0)
		return false;

	// 단순히 복사해주기 
	m_ItemInfo = RuntimeData;

	switch (m_ItemInfo.ItemStaticData.Type)
	{
	case static_cast<int32>(EItemType::EquipmentItem):
	{
		// 스켈레탈 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
		USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *m_ItemInfo.ItemStaticData.EquipmentMesh->GetPathName(), NULL, LOAD_None, NULL);

		if (SkeletalMesh != nullptr)
		{
			// (임시) 여기서 필요할때 장비메시 컴포넌트를 생성해줘보자.
			if (m_EquipmentMeshComp == nullptr)
			{
				m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
				m_EquipmentMeshComp->SetupAttachment(RootComponent);
			}

			m_EquipmentMeshComp->SetSkeletalMesh(SkeletalMesh);
			m_EquipmentMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_EquipmentMeshComp->SetSimulatePhysics(true);
			m_EquipmentMeshComp->SetWorldScale3D(m_EquipmentMeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.

		}
		break;
	}
	case static_cast<int32>(EItemType::ConsumableItem):
	case static_cast<int32>(EItemType::Non_ConsumableItem):
	case static_cast<int32>(EItemType::EtcItem):
	{
		// 스테틱 메쉬 생성과정 [ 직접 경로 만들어서 붙여 주는중 ] 
		UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *m_ItemInfo.ItemStaticData.ItemMesh->GetPathName(), NULL, LOAD_None, NULL);

		if (StaticMesh != nullptr)
		{
			m_MeshComp->SetStaticMesh(StaticMesh);
			//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
			m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_MeshComp->SetSimulatePhysics(true);
			m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.
		}
		break;
	}
	default:
	{
		// NullItem, End 일때는 실패를 띄우자.
		return false;
	}
	}

	// 설명서 추가하기 . [ 아이템이 다 세팅된 다음에 해야한다.
	m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "), *m_ItemInfo.ItemStaticData.Name);

	return true;
}

void AMyItem::Init()
{
	// 부모가 가지고 있는 멤버들 세팅
	Super::Init();
	// 자식이 가지고 있는 멤버들 세팅



}

void AMyItem::OnInteract()
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
		OnInteract();
	}

}

void AMyItem::OnPickedUp()
{
	// OFF 상태로 숨겨주기 . 
	AMyInteractable::m_Effect->Activate(true);
	m_IsGround = false;

	// 아이템 가려주는 부분.
	ApplyHidden(true);
	m_HelpTextComp->SetVisibility(false);

	// 트리거 세팅 
	m_Trigger->SetSimulatePhysics(false);
	m_Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
// (임시) 다시 만들어야 한다. 다시 놓을때 아이템 값을 세팅해줘야한다.  
void AMyItem::Replace(const FVector Pos)
{
	// 부모가 가지고 있는 멤버들 세팅
	Super::Replace(Pos);
	// 자식이 가지고 있는 멤버들 세팅
	m_IsGround = true;

	// 스테틱 메시 세팅
	if (m_MeshComp->GetStaticMesh() != nullptr)
	{
		m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// 스케일 두배로.
	}
	// 스켈레탈 메시 세팅
	if (m_EquipmentMeshComp != nullptr)
	{
		m_EquipmentMeshComp->SetWorldTransform(FTransform(Pos));
	}

}

void AMyItem::ApplyHidden(const bool bHide)
{
	Super::ApplyHidden(bHide);

	// Skeletal Mesh Component
	if (m_EquipmentMeshComp != nullptr)
	{
		m_EquipmentMeshComp->SetHiddenInGame(!m_bVisiable);
		m_EquipmentMeshComp->SetVisibility(m_bVisiable);
		m_EquipmentMeshComp->SetSimulatePhysics(m_bVisiable);
	}
}

void AMyItem::AcquireObject()
{
	Super::AcquireObject();

}

void AMyItem::ReleaseObject()
{
	Super::ReleaseObject();

}

void AMyItem::ResetObject()
{
	Super::ResetObject();
	// 아이템을 리젯시키기


}

bool AMyItem::RemoveObject()
{
	// 오브젝트가 최종적으로 사라질때 호출되는 함수 
	// Subsystem에서 제어하자.

	return Destroy();
}


//void AMyItem::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	AMyInteractable::OnCharacterBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//}
//
//void AMyItem::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	AMyInteractable::OnCharacterEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
//}


