// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCharacter.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceBase.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyAIController.h"
#include "PawnWidget.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Data/MyGameSubsystem.h"

#include "DamageFontWidget.h"
#include "EmotionWidget.h"
#include "CharacterComponent/MyStatComponent.h"
#include "CharacterComponent/MyInventoryComponent.h"

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"

#include "Utill/RandomSystem.h"

// (임시)
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ANPCharacter::ANPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 태그를 추가하기.
	Tags.Add(FName("NPC"));

	//// 캡슐 사이즈는 필요하면 다시 설정해주자.
	//// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Gauge bar
	m_GaugeBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("GAUGEBAR"));
	m_GaugeBar->SetupAttachment(GetMesh());
	m_GaugeBar->SetRelativeLocation(FVector{ 0.f,0.f,200.f });
	m_GaugeBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Game/TopDown/Blueprints/UI/WBP_GaugeBar.WBP_GaugeBar_C"));
	if (UW.Succeeded())
	{
		m_GaugeBar->SetWidgetClass(UW.Class);
		m_GaugeBar->SetDrawSize(FVector2D{ 200.f,50.f });
		
	}

	// Emotion
	m_Emotion = CreateDefaultSubobject<UWidgetComponent>(TEXT("EMOTION"));
	m_Emotion->SetupAttachment(GetMesh());
	m_Emotion->SetRelativeLocation(FVector{ 0.f,0.f,250.f });
	m_Emotion->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> EW(TEXT("/Game/TopDown/Blueprints/UI/WBP_Emotion.WBP_Emotion_C"));
	if (EW.Succeeded())
	{
		m_Emotion->SetWidgetClass(EW.Class);
		m_Emotion->SetDrawSize(FVector2D{ 25.f,25.f });
	}
	


	// Setup Components
	m_pStatComp = CastChecked<UMyStatComponent>(CreateDefaultSubobject<UMyStatComponent>(TEXT("StatComp")));


	// AI Set
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
// Called when the game starts or when spawned
void ANPCharacter::BeginPlay()
{
	Super::BeginPlay();

	//// Data Setup
	//SetupCharacterData();

	// (임시) 충돌 채널 설정해주기.
	m_Ally = ECollisionChannel::ECC_GameTraceChannel2;
	m_Enemy = ECollisionChannel::ECC_GameTraceChannel4;

	
	// 여기에서 초기 데이터를 세팅해주자 ( 월드가 생기고 나서 사용해야한다. ) 
	SetupCharacterData();



	//DEBUG 
	GetWorld()->GetTimerManager().SetTimer(m_hDebug, this, &ANPCharacter::Debug, 0.1f, true);

}

void ANPCharacter::SetupCharacterData()
{
	// 여기서 각 컴포넌트의 값들을 세팅해준다.

	m_pStatComp->SetupData(*m_CharacterName);
	SetMainMesh();
	m_pInventoryComp->SetupData(*m_CharacterName);

}
// Called every frame
void ANPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Debug


}
// Called to bind functionality to input

void ANPCharacter::PostInitializeComponents()
{
	// 컴포넌트 들이 초기화된 이후 
	Super::PostInitializeComponents();


	if (m_pAnimInstance != nullptr)
	{
		m_pAnimInstance->m_OnHitEnd.AddUObject(this, &ACharacterBase::OnHitMontageEnded);

		m_pAnimInstance->m_OnDeathEnd.AddUObject(this, &ANPCharacter::DeathEnd);
		m_pAnimInstance->m_OnAnimEnd.AddUObject(this, &ANPCharacter::OnBehaviorMontageEnded);

	}
	else
	{
		// AnimIstance가 생성이 안되어서 생기는 문제일거같다. 
	}

	// 스텟에서 OnDeathCheck가 Broadcast 된다면 Death함수 호출 
	m_pStatComp->OnDeathCheck.AddUObject(this, &ANPCharacter::Death);


	// Combat Stat				// 테스팅용 값이다. 값을 받아서 객체마다 다르게 세팅해야한다.
	m_AttackRange = m_pStatComp->GetTotalStat().AttackRange;
	m_AttackRadius = m_pStatComp->GetTotalStat().AttackRadius;
	m_RangedRadius = 150.f;
	m_DetectionRadius = 800.f;
	m_VisibleRadius = 1200.f;



	m_GaugeBar->InitWidget();
	m_Emotion->InitWidget();

	// 이 방식 보다는 Bind하는 방식이 좋을듯하다. 
	//CastChecked<UEmotionWidget>(m_Emotion->GetWidget())->SetOwningActor(this);
	CastChecked<UEmotionWidget>(m_Emotion->GetWidget())->BindFunction(this);

	// TODO
	auto GaugeWidget = Cast<UPawnWidget>(m_GaugeBar->GetUserWidgetObject());
	if (GaugeWidget != nullptr)
	{
		GaugeWidget->SetOwningActor(this);
		GaugeWidget->BindHp(m_pStatComp);
	}



	//ensure(NPCMeshes.Num() > 0);
	//int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	//NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &ANPCharacter::NPCMeshLoadCompleted));


}

#pragma region Behavior Interface
void ANPCharacter::SetAggroGauge(const uint8 AggroGauge)
{
	m_AggroGauge = FMath::Clamp(m_AggroGauge + AggroGauge, 0, 100); 
	m_ChangeAggroGauge.Broadcast(m_AggroGauge);
}

void ANPCharacter::OnIdle()
{
	SetState(EBehaviorState::Idle);
	SetNextState(EBehaviorState::Idle);
	SetMode(ECharacterMode::Idle);

	ResetMoveSpeed();
	LookDirection(0.f);


}

void ANPCharacter::OnBattle()
{
	SetState(EBehaviorState::Battle);
	SetNextState(EBehaviorState::Battle);
	SetMode(ECharacterMode::Battle);

	SetMoveSpeed((m_OriginMoveSpeed *0.5f));
}

void ANPCharacter::Death()
{
	if (m_CharacterState < EBehaviorState::Die)
	{
		Super::Death();
	
		m_GaugeBar->SetVisibility(false);

		// 컨트롤러 제어 OFF
		AMyAIController* MyAIController = Cast<AMyAIController>(GetController());
		if (MyAIController)
		{
			MyAIController->StopAI();
		}
	}
}

void ANPCharacter::DeathEnd()
{
	if(m_CharacterState == EBehaviorState::Die)
	{
		this->SetActorEnableCollision(false);

#pragma region DEBUG
		//if (GEngine) 
		//{
		//	GEngine->AddOnScreenDebugMessage
		//	(-1, 1.0f, FColor::Red, FString::FromInt(static_cast<int32>(m_fCurrentOpacity * 100.f)));
		//}
#pragma endregion
		if (FMath::IsNearlyEqual(m_fCurrentOpacity,1.f))
		{
			GetWorld()->GetTimerManager().SetTimer(m_hDeathTimer, this, &ANPCharacter::DeathEnd, 1.f, true);
		}
		else if (m_fCurrentOpacity <= 0.f)
		{
			m_fCurrentOpacity = 1.f;
			SetState(EBehaviorState::End);
			SetNextState(EBehaviorState::End);
			OnDeathEnd.Broadcast();
			GetWorld()->GetTimerManager().ClearTimer(m_hDeathTimer);

			// 메쉬가 가지고 있는 부착된 자식들을 모두 숨김 처리.
			TArray<USceneComponent*> AttachedActors = GetMesh()->GetAttachChildren();
			for (auto elem : AttachedActors)
			{
				elem->SetVisibleFlag(false);
				elem->SetHiddenInGame(true);
			}

			GetMesh()->SetHiddenInGame(true);

			return;
		}
		else
		{
				m_fCurrentOpacity -= 0.05f;

		}


	} 
}

void ANPCharacter::OnDefense()
{
	m_pAnimInstance->PlayMontage(static_cast<uint8>(EMontageList::BehaviorMontage),m_AnimationSpeed);
	m_pAnimInstance->JumpToSection(FName(TEXT("Defense")), static_cast<uint8>(EMontageList::BehaviorMontage));

	SetState(EBehaviorState::Defense);
	SetNextState(EBehaviorState::Battle);
}

void ANPCharacter::StopDefense()
{
	m_pAnimInstance->StopMontage(static_cast<uint8>(EMontageList::BehaviorMontage));

	SetState(m_NextCharacterState);
}

void ANPCharacter::Defense_Hit()
{
	SetState(EBehaviorState::Hit);

	m_pAnimInstance->PlayMontage(static_cast<uint8>(EMontageList::BehaviorMontage),m_AnimationSpeed);
	m_pAnimInstance->JumpToSection(FName(TEXT("Defense_Hit")), static_cast<uint8>(EMontageList::BehaviorMontage));

	// Effect
	Effect_Flick(FLinearColor::Black);
}

void ANPCharacter::OnDetect()
{
	if (m_CharacterState == EBehaviorState::Idle)
	{
		SetState(EBehaviorState::Detect);
		SetNextState(EBehaviorState::Alert);
		//m_pAnimInstance->PlayBehaviorMontage(m_AnimationSpeed);
		//m_pAnimInstance->JumpToSection_Behavior(TEXT("Detect"));
		
		// Effect
		Effect_Flick(FLinearColor::White);
	
		// (임시) 종료하는 시간을 임의로 넣주었다.
		GetWorld()->GetTimerManager().SetTimer(m_hDetectTimer, this, &ANPCharacter::DetectEnd, 3.f, false);
	}
}

void ANPCharacter::DetectEnd()
{
	SetState(m_NextCharacterState);
	OnDetectEnd.Broadcast();
}

void ANPCharacter::OnAlert()
{
	const uint8 ChangePointToBattleMode = 10U;
	if (m_AggroGauge <= ChangePointToBattleMode)
	{
		// 경계 상태의 대기시간을 지정하기 위해서 타이머 활용
		GetWorld()->GetTimerManager().SetTimer(m_hAlertTimer, this, &ANPCharacter::OnAlert, 1.0f, true);
		
		// (임시) 이동속도 변경 .
		SetMoveSpeed((m_OriginMoveSpeed * 0.25f));

		int32 Rand = (FMath::RandRange(1, 100) / 100);
		SetAggroGauge(m_AggroGauge + Rand);
	}
	else 
	{
		// 전투 결정
		OnBattle();
		OnAlertEnd.Broadcast();

		GetWorld()->GetTimerManager().ClearTimer(m_hAlertTimer);
		// 경계가 끝나면 바라보는 방향도 세팅
		LookDirection(0);
	}

}

void ANPCharacter::OnCommand(uint8 Command)
{
	// 그냥 바로 다이렉트로 하는게 맞을것인가.
	// 지금처럼 캐릭터안에 있는 OnCommand를 거치는게 맞을것인가.
	OnChangeCommand.Broadcast(Command);
}

bool ANPCharacter::IsOnTriggerEscape()
{
	// (임시) 0.2f 는 상수이니까 도망의 조건 값을 매개변수로 받거나  멤버 변수를 지정해서 세팅해줘야할듯하다. 
	return (m_pStatComp->GetHPRatio() <= 0.2f);
}
#pragma endregion

void ANPCharacter::HighlightActor()
{
	//DEBUG
	UE_LOG(LogTemp, Log, TEXT(" MouseOverlap : Enter "));
	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
	
}

void ANPCharacter::UnHighlightActor()
{
	//DEBUG
	UE_LOG(LogTemp, Log, TEXT(" MouseOverlap : Leave "));

	GetMesh()->SetRenderCustomDepth(false);
	
}

#pragma region Animation Function
void ANPCharacter::OnBehaviorMontageEnded()
{	
	switch (m_CharacterState)
	{
		case EBehaviorState::Defense:
		{
			SetState(m_NextCharacterState);
			OnDefenseEnd.Broadcast();

			break;
		}
		default :
		{
			
			break;
		}
	}
}
#pragma endregion

float ANPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_CharacterState == EBehaviorState::Die)
		return 0.f;

	// NextState
	SetNextState(EBehaviorState::Battle);
	SetMode(ECharacterMode::Battle);

	// Overcome 극복하다. 10%의 확률로 회피 Dodge
	int32 Random = FMath::RandRange(0, 9);
	if (Random < 1)
	{
		// 저항하면 바로 전투 돌입.
		SetState(EBehaviorState::Battle);
		DamageAmount = 0.f;
	}
	else 
	{
		if (m_CharacterState == EBehaviorState::Defense)
		{
			// 방어 상태. 
			DamageAmount *= 0.5f;
			
			// 방어 상태가 종료되었다는걸 알린다.
			OnDefenseEnd.Broadcast();
			//SetState(EBehaviorState::Hit);	// Defense_Hit함수 안에다가 상태 전환을 넣었다.
			Defense_Hit();
			//m_pStatComp->OnHPChanged.AddLambda([this, DamageAmount]()
			//	{
			//		if (DamageAmount > 0)
			//		{
			//			this->Defense_Hit();
			//		}
			//	});

		}
		else
		{
			// Change State
			//SetState(EBehaviorState::Hit);	// Hit함수 안에다가 상태 전환을 넣었다.
			OnHit();
			//m_pStatComp->OnHPChanged.AddLambda([this, DamageAmount]()
			//	{
			//		if (DamageAmount > 0)
			//		{
			//			this->OnHit();
			//		}
			//	});
		}
		


	}

	// Set Stat
	m_pStatComp->OnAttacked(DamageAmount);

	// Set Aggro Gauge (임시) 상수값 10 
	m_AggroGauge = FMath::Clamp(m_AggroGauge+10U, 0U, 100U);

	// DamageFont 
	UMyGameSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();
	if (Subsystem != nullptr)
	{
		// 소켓명을 Root를 달아야 한다.
		FVector Root = ANPCharacter::GetMesh()->GetSocketLocation(TEXT("Root"));
		FVector2D Root2D{ 0.0,-200.0 };
		Root2D.X += FMath::RandRange(-300.0, 300.0);
		Root2D.Y += FMath::RandRange(-50.0, 50.0);

		if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), Root, Root2D))
		{
			UUserWidget* DFW = Subsystem->GetDamageFont();
			if (DFW != nullptr)
			{
				DFW->SetPositionInViewport(Root2D, true);
				DFW->AddToViewport();
				// HitType을 여기서 정해줘야한다  
				Cast<UDamageFontWidget>(DFW)->TakeDamage(static_cast<int32>(DamageAmount), EHitType::HitNormal);	// float -> int
			}
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DamageFont Fail (SubSystem Fail)"));
	}


	return DamageAmount;

}

void ANPCharacter::ApplyHidden(const bool bHide)
{
	Super::ApplyHidden(bHide);
}

void ANPCharacter::SetupMonsterWidget(UMyUserWidget* InUserWidget)
{



}
/*
void ANPCharacter::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}
*/
void ANPCharacter::Debug()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, strState[m_CharacterState]);

	/* 게임 모드 체크용 */
	//FString str{ TEXT("Null") };
	//switch (m_Mode)
	//{
	//case ECharacterMode::Idle :
	//{
	//	str = TEXT("IDLE");
	//	break;
	//}
	//case ECharacterMode::Battle:
	//{
	//	str = TEXT("BATTLE");
	//	break;
	//}
	//case ECharacterMode::StopAI:
	//{
	//	str = TEXT("StopAI");
	//	break;
	//}
	//}
	//
	//UE_LOG(LogTemp, Log, TEXT("Mode : %s"), *str);
}

