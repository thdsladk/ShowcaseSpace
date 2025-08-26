// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInteractable.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Interface/InteractableInterface.h"
#include "ItemHelpTip.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "MyTest_TopDown/Physics/MyCollision.h"

// Sets default values
AMyInteractable::AMyInteractable()
	:m_bVisiable(true), m_bInCharacter(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_HelpTextComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HelpTip"));
	m_Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));


	m_Trigger->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	m_Trigger->SetCollisionProfileName(CPROFILE_ITEMBOX);


	m_InteractableHelpText = TEXT("Press F to interact with item");
	

	
	//m_HelpTextComp->SetupAttachment(m_Trigger);
	m_HelpTextComp->SetRelativeLocation(FVector{ 0.f,0.f,200.f });
	m_HelpTextComp->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TopDown/Blueprints/BP_ItemHelpTip.BP_ItemHelpTip_C'"));
	if (UW.Succeeded())
	{
		m_HelpTextComp->SetWidgetClass(UW.Class);
		m_HelpTextComp->SetDrawSize(FVector2D{ 4.f,1.f });
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_CoinBurst.NS_CoinBurst"));
	if (NS.Succeeded())
	{
		m_Effect->SetAsset(NS.Object);
		m_Effect->bAutoActivate = false;
	}


	// 컴포넌트 구조
	RootComponent = m_Trigger;
	m_MeshComp->SetupAttachment(RootComponent);
	m_HelpTextComp->SetupAttachment(RootComponent);
	m_Effect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에 생성될때 표시되도록 한다. 
	ApplyHidden(false);
}

void AMyInteractable::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Bind Overlap Function
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyInteractable::OnCharacterBeginOverlap);
	m_Trigger->OnComponentEndOverlap.AddDynamic(this, &AMyInteractable::OnCharacterEndOverlap);

	m_HelpTextComp->InitWidget();

	
	auto HelpWidget = Cast<UItemHelpTip>(m_HelpTextComp->GetUserWidgetObject());
	if (HelpWidget)
		HelpWidget->BindHelpTip(TEXT(""), m_InteractableHelpText);
	// 첫번째 인자가 비어있는 이유는 상호작용객체는 아직 Name을 가지고 있지않아서 .. 

	// HelpText 안보이게 끄기.

	m_HelpTextComp->SetVisibility(false);

}

#pragma region Collision Function
void AMyInteractable::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{  
	
	IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(OtherActor);
	if (InteractableInterface != nullptr)
	{
		//  상호작용 중인 객체가 존재하지 않으면 
		if (InteractableInterface->HasCurrentInteractable() == false)
		{
			InteractableInterface->SetCurrentInteractable(this);
			
			// 캐릭터와 충돌 시작
			m_bInCharacter = true;

			if (m_bVisiable == true)
			{
				m_HelpTextComp->SetVisibility(true);
			}
			else
			{
				m_HelpTextComp->SetVisibility(false);
			}
		}
	}
	
}

void AMyInteractable::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(OtherActor);
	if (InteractableInterface != nullptr)
	{
		InteractableInterface->SetCurrentInteractable(nullptr);

		// 캐릭터와 충돌 끝
		m_bInCharacter = false;

		m_HelpTextComp->SetVisibility(false);	
		
	}

}
#pragma endregion

void AMyInteractable::Init()
{
	// ON 상태로 표시. 

	m_HelpTextComp->SetActive(true);

	ApplyHidden(false);

	m_Trigger->SetSimulatePhysics(true);
	//m_Trigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_Trigger->SetCollisionProfileName(CPROFILE_ITEMBOX);
}

void AMyInteractable::OnInteract()
{
	m_Effect->Activate(true);
}

void AMyInteractable::Interact_Implementation()
{

}

void AMyInteractable::Replace(FVector Pos)
{
	Init();
	SetActorTransform(FTransform(Pos));
	if (m_MeshComp->GetStaticMesh() != nullptr)
	{
		m_MeshComp->SetWorldTransform(FTransform(Pos));
	}
}

void AMyInteractable::ResetObject()
{



}

bool AMyInteractable::RemoveObject()
{
	// 아이템 갯수가 0이 되어서 소멸을 호출 해야한다 .
	// 1. 소멸 시키기    2. 아이템 매니저를 만들어서 0이된 아이템은 매니저에서 보관 
	// (임시) 지금은 소멸 시키도록 해둔다.
	// return   IsPendingKillPending 
	
	return Destroy();
}

void AMyInteractable::ApplyHidden(const bool bHide)
{
	AActor::SetHidden(bHide);

	m_bVisiable = !bHide;
	SetActorHiddenInGame(!m_bVisiable);
	SetActorEnableCollision(m_bVisiable);
	SetActorTickEnabled(m_bVisiable);

	// Static Mesh Component
	if (m_MeshComp->GetStaticMesh() != nullptr)
	{
		m_MeshComp->SetHiddenInGame(!m_bVisiable);
		m_MeshComp->SetVisibility(m_bVisiable);
		m_MeshComp->SetSimulatePhysics(m_bVisiable);
	}
}

void AMyInteractable::AcquireObject()
{
	Init();

}

void AMyInteractable::ReleaseObject()
{
	// Render Off
	ApplyHidden(true);
}
