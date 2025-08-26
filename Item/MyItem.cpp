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

	// �ʿ��Ҷ� �����ؼ� ����.
	//if (m_EquipmentMeshComp == nullptr)
	//{
	//	m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	//	m_EquipmentMeshComp->SetupAttachment(RootComponent);
	//}
}

void AMyItem::BeginPlay()
{
	Super::BeginPlay();

	// ���忡 �����ɶ� ǥ�õǵ��� �Ѵ�. 
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

	// m_ItemInfo.Count �� �����Ϳ��� �־�����. 

	switch (m_ItemInfo.ItemStaticData.Type)
	{
	case static_cast<int32>(EItemType::EquipmentItem):
	{
		// ���̷�Ż �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
		if (m_ItemInfo.ItemStaticData.EquipmentMesh.IsValid() == false)
		{
			m_ItemInfo.ItemStaticData.EquipmentMesh.LoadSynchronous();
		}
		FString Path = m_ItemInfo.ItemStaticData.EquipmentMesh->GetPathName();
		USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *(Path), NULL, LOAD_None, NULL);

		if (SkeletalMesh != nullptr)
		{
			// �ʿ��� ��츸 �����ؼ� ������ 
			if (m_EquipmentMeshComp == nullptr)
			{
				// 1. NewObject�� ����
				m_EquipmentMeshComp = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("EquipmentMesh_Runtime"));
				// 2. ���Ϳ� ������Ʈ ������ �ο�
				AddInstanceComponent(m_EquipmentMeshComp);
				// 3. ����ġ ����
				m_EquipmentMeshComp->SetupAttachment(RootComponent);
				// 4. ���忡 ��� (����/ƽ/���� �ݿ� ����)
				m_EquipmentMeshComp->RegisterComponent();
			}

			m_EquipmentMeshComp->SetSkeletalMesh(SkeletalMesh);
			m_EquipmentMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_EquipmentMeshComp->SetSimulatePhysics(true);
			m_EquipmentMeshComp->SetWorldScale3D(m_EquipmentMeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.

		}
		break;
	}
	case static_cast<int32>(EItemType::ConsumableItem):
	case static_cast<int32>(EItemType::Non_ConsumableItem):
	case static_cast<int32>(EItemType::EtcItem):
	{
		// ����ƽ �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
		UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *m_ItemInfo.ItemStaticData.ItemMesh->GetPathName(), NULL, LOAD_None, NULL);

		if (StaticMesh != nullptr)
		{
			m_MeshComp->SetStaticMesh(StaticMesh);
			//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
			m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_MeshComp->SetSimulatePhysics(true);
			m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.
		}
		break;
	}
	default:
	{
		// NullItem, End �϶��� ���и� �����.
		return;
	}
	}

	// ���� �߰��ϱ� . [ �������� �� ���õ� ������ �ؾ��Ѵ�.
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
				// ���̷�Ż �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
				USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *ItemData->EquipmentMesh->GetPathName(), NULL, LOAD_None, NULL);


				if (SkeletalMesh != nullptr)
				{
					// (�ӽ�) ���⼭ �ʿ��Ҷ� ���޽� ������Ʈ�� �������ຸ��.
					if (m_EquipmentMeshComp == nullptr)
					{
						m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
						m_EquipmentMeshComp->SetupAttachment(RootComponent);
					}

					m_EquipmentMeshComp->SetSkeletalMesh(SkeletalMesh);
					m_EquipmentMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
					m_EquipmentMeshComp->SetSimulatePhysics(true);
					m_EquipmentMeshComp->SetWorldScale3D(m_EquipmentMeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.
				}
				break;
			}
			case static_cast<int32>(EItemType::ConsumableItem):
			case static_cast<int32>(EItemType::Non_ConsumableItem):
			case static_cast<int32>(EItemType::EtcItem):
			{
				// ����ƽ �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
				UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *ItemData->ItemMesh->GetPathName(), NULL, LOAD_None, NULL);

				if (StaticMesh != nullptr)
				{
					m_MeshComp->SetStaticMesh(StaticMesh);
					//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
					m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
					m_MeshComp->SetSimulatePhysics(true);
					m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.
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

			// ���� �߰��ϱ� . [ �������� �� ���õ� ������ �ؾ��Ѵ�.
			m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "), *m_ItemInfo.ItemStaticData.Name);

		}
	}


}

bool AMyItem::InitData(FItemRuntimeData& RuntimeData)
{
	// �ܺο��� ������ �����͸� �޾ƿ´�. 
	if (RuntimeData.Count <= 0)
		return false;

	// �ܼ��� �������ֱ� 
	m_ItemInfo = RuntimeData;

	switch (m_ItemInfo.ItemStaticData.Type)
	{
	case static_cast<int32>(EItemType::EquipmentItem):
	{
		// ���̷�Ż �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
		USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *m_ItemInfo.ItemStaticData.EquipmentMesh->GetPathName(), NULL, LOAD_None, NULL);

		if (SkeletalMesh != nullptr)
		{
			// (�ӽ�) ���⼭ �ʿ��Ҷ� ���޽� ������Ʈ�� �������ຸ��.
			if (m_EquipmentMeshComp == nullptr)
			{
				m_EquipmentMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
				m_EquipmentMeshComp->SetupAttachment(RootComponent);
			}

			m_EquipmentMeshComp->SetSkeletalMesh(SkeletalMesh);
			m_EquipmentMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_EquipmentMeshComp->SetSimulatePhysics(true);
			m_EquipmentMeshComp->SetWorldScale3D(m_EquipmentMeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.

		}
		break;
	}
	case static_cast<int32>(EItemType::ConsumableItem):
	case static_cast<int32>(EItemType::Non_ConsumableItem):
	case static_cast<int32>(EItemType::EtcItem):
	{
		// ����ƽ �޽� �������� [ ���� ��� ���� �ٿ� �ִ��� ] 
		UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *m_ItemInfo.ItemStaticData.ItemMesh->GetPathName(), NULL, LOAD_None, NULL);

		if (StaticMesh != nullptr)
		{
			m_MeshComp->SetStaticMesh(StaticMesh);
			//m_MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
			m_MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
			m_MeshComp->SetSimulatePhysics(true);
			m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.
		}
		break;
	}
	default:
	{
		// NullItem, End �϶��� ���и� �����.
		return false;
	}
	}

	// ���� �߰��ϱ� . [ �������� �� ���õ� ������ �ؾ��Ѵ�.
	m_InteractableHelpText = FString::Printf(TEXT("%s : Press F to Pick up "), *m_ItemInfo.ItemStaticData.Name);

	return true;
}

void AMyItem::Init()
{
	// �θ� ������ �ִ� ����� ����
	Super::Init();
	// �ڽ��� ������ �ִ� ����� ����



}

void AMyItem::OnInteract()
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
		OnInteract();
	}

}

void AMyItem::OnPickedUp()
{
	// OFF ���·� �����ֱ� . 
	AMyInteractable::m_Effect->Activate(true);
	m_IsGround = false;

	// ������ �����ִ� �κ�.
	ApplyHidden(true);
	m_HelpTextComp->SetVisibility(false);

	// Ʈ���� ���� 
	m_Trigger->SetSimulatePhysics(false);
	m_Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
// (�ӽ�) �ٽ� ������ �Ѵ�. �ٽ� ������ ������ ���� ����������Ѵ�.  
void AMyItem::Replace(const FVector Pos)
{
	// �θ� ������ �ִ� ����� ����
	Super::Replace(Pos);
	// �ڽ��� ������ �ִ� ����� ����
	m_IsGround = true;

	// ����ƽ �޽� ����
	if (m_MeshComp->GetStaticMesh() != nullptr)
	{
		m_MeshComp->SetWorldScale3D(m_MeshComp->GetComponentScale() * m_ItemInfo.ItemStaticData.Scale);	// ������ �ι��.
	}
	// ���̷�Ż �޽� ����
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
	// �������� ������Ű��


}

bool AMyItem::RemoveObject()
{
	// ������Ʈ�� ���������� ������� ȣ��Ǵ� �Լ� 
	// Subsystem���� ��������.

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


