// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCharacter.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_Goblin.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyAIController.h"
#include "MonsterStatComponent.h"
#include "PawnWidget.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

#include "MyEquipment.h"
#include "DamageFontWidget.h"
#include "EmotionWidget.h"

#include "Engine/AssetManager.h"
#include "NavigationSystem.h"

// Sets default values
ANPCharacter::ANPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// 캡슐 사이즈는 필요하면 다시 설정해주자.
	//// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Skeletal Mesh
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
	//
	//if (SM.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SM.Object);
	//}

	// Stat
	m_pStatComp = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("Stat"));


	// HP bar
	m_HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	m_HpBar->SetupAttachment(GetMesh());
	m_HpBar->SetRelativeLocation(FVector{ 0.f,0.f,200.f });
	m_HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Game/TopDown/UI/WBP_HPBar.WBP_HPBar_C"));
	if (UW.Succeeded())
	{
		m_HpBar->SetWidgetClass(UW.Class);
		m_HpBar->SetDrawSize(FVector2D{ 200.f,50.f });
	}

	// Emotion
	m_Emotion = CreateDefaultSubobject<UWidgetComponent>(TEXT("EMOTION"));
	m_Emotion->SetupAttachment(GetMesh());
	m_Emotion->SetRelativeLocation(FVector{ 0.f,0.f,250.f });
	m_Emotion->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> EW(TEXT("/Game/TopDown/UI/WBP_Emotion.WBP_Emotion_C"));
	if (EW.Succeeded())
	{
		m_Emotion->SetWidgetClass(EW.Class);
		m_Emotion->SetDrawSize(FVector2D{ 25.f,25.f });
	}
	
	static ConstructorHelpers::FClassFinder<UDamageFontWidget> DFW(TEXT("/Game/TopDown/Blueprints/BP_DamageFont.BP_DamageFont_C"));
	if (DFW.Succeeded())
	{
		m_DamageFont = DFW.Class;
	}



	// AI Set
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
// Called when the game starts or when spawned
void ANPCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	m_NPCType = FName("Goblin_Red-1");
	FMonsterData* MonsterData = Cast<UMyGameInstance>(GetGameInstance())->GetMonsterStatData(m_NPCType.ToString());
	//// DuplicateObject를 통해서 생성한 메시객체는 어디서 메모리 관리를 할까 직접 멤버관리를 하는것일까? 
	//// UObject의 자식이라서 일단 가비지 컬렉터가 관리할것으로 보인다. 
	//GetMesh()->SetSkeletalMesh(DuplicateObject<USkeletalMesh>(MonsterData->SkeletalMesh, this));

	//AttachmentEquipment Weapon1
	FName WeaponSocket(TEXT("Weapon_Socket"));
	m_EquipmentLeft = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());

	if (nullptr != m_EquipmentLeft)
	{
		m_EquipmentLeft->SetItem(MonsterData->LItemID);
		m_EquipmentLeft->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
		m_EquipmentLeft->OnEquip();

		m_EquipmentLeft->FinishSpawning(FTransform());
	}

	//AttachmentEquipment Weapon2
	FName ShieldSocket(TEXT("Shield_Socket"));
	m_EquipmentRight = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());

	if (nullptr != m_EquipmentRight)
	{
		m_EquipmentRight->SetItem(MonsterData->RItemID);
		m_EquipmentRight->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ShieldSocket);
		m_EquipmentRight->OnEquip();

		m_EquipmentRight->FinishSpawning(FTransform());
	}



	//DEBUG 
	GetWorld()->GetTimerManager().SetTimer(m_hDebug, this, &ANPCharacter::Debug, 0.1f, true);

}
// Called every frame
void ANPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Debug

	//UE_LOG(LogTemp, Log, TEXT("MonState : %d"),m_CharacterState);
}
// Called to bind functionality to input
void ANPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 아직은 NPC는 인풋 관련 컴포넌트를 받을 생각이 없다. 

}

void ANPCharacter::PostInitializeComponents()
{
	// 컴포넌트 들이 초기화된 이후 
	Super::PostInitializeComponents();


	m_AnimInstance = CastChecked<UAnimInstance_Goblin>(GetMesh()->GetAnimInstance());
	// 애니메이션에서 몽타주가 끝나는 시점에 호출하는 델리게이트라서 꺼두었다.
	//m_AnimInstance->OnMontageEnded.AddDynamic(this, &ANPCharacter::OnAttackMontageEnded);
	// 애니메이션 특정 지점마다 호출하도록 해서 원하는 세션 또는 지점에서 호출 하도록 유도
	m_AnimInstance->m_OnAttackHit.AddUObject(this, &ANPCharacter::AttackCheck);
	m_AnimInstance->m_OnAttackEnd.AddUObject(this, &ANPCharacter::AttackEnd);

	m_AnimInstance->m_OnDeathPoint.AddUObject(this, &ANPCharacter::OnDeathMontageEnded);

	// 스텟에서 OnDeathCheck가 Broadcast 된다면 Death함수 호출 
	m_pStatComp->OnDeathCheck.AddUObject(this, &ANPCharacter::Death);

	// Combat Stat				// 테스팅용 값이다. 값을 받아서 객체마다 다르게 세팅해야한다.
	m_AttackRange = 50.f;
	m_AttackRadius = 50.f;
	m_DefenseRadius = 150.f;
	m_DetectionRadius = 800.f;
	m_VisibleRadius = 1200.f;


	m_HpBar->InitWidget();
	m_Emotion->InitWidget();

	// TODO
	auto HpWidget = Cast<UPawnWidget>(m_HpBar->GetUserWidgetObject());
	if (HpWidget)
		HpWidget->BindHp(m_pStatComp);


	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &ANPCharacter::NPCMeshLoadCompleted));


}

void ANPCharacter::OnIdle()
{
	m_CharacterState = EBehaviorState::Idle;
	m_NextCharacterState = EBehaviorState::Idle;
	m_Mode = ECharacterMode::Idle;

	ResetMoveSpeed();
	LookDirection(0.f);


}

void ANPCharacter::OnBattle()
{
	m_CharacterState = EBehaviorState::Battle;
	m_NextCharacterState = EBehaviorState::Battle;
	m_Mode = ECharacterMode::Battle;

	SetMoveSpeed((m_OriginMoveSpeed *0.5f));
}

void ANPCharacter::Attack()
{
	m_AnimInstance->PlayAttackMontage();
	m_AnimInstance->JumpToSection_Attack(m_AttackIndex);
	m_AttackIndex = (m_AttackIndex + 1) % 3; // Attack 모션이 2개 이므로

	m_CharacterState = EBehaviorState::Attacking;
	m_NextCharacterState = EBehaviorState::Battle;
}

void ANPCharacter::AttackCheck()
{
	if (m_CharacterState != EBehaviorState::Attacking)
		return;

	FHitResult HitResult; 
	FCollisionQueryParams Params(NAME_None, false, this);


	FVector Vec = GetActorForwardVector() * m_AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = m_AttackRange * 0.5f + m_AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();


	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * m_AttackRange,
		Rotation,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(m_AttackRadius,HalfHeight),
		Params);


	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, m_AttackRadius,
		Rotation, DrawColor, false, 2.f);


	if (bResult && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(m_pStatComp->GetAttack(), DamageEvent, GetController(), this);

	}
}

void ANPCharacter::AttackEnd()
{
	if (m_CharacterState == EBehaviorState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_AnimInstance->StopAttackMontage();
		m_CharacterState = m_NextCharacterState;
	}
}

void ANPCharacter::Death()
{
	if (m_CharacterState < EBehaviorState::Die)
	{
		m_CharacterState = EBehaviorState::Die;
		m_NextCharacterState = EBehaviorState::Die;
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(TEXT("Death_B"));

		m_AnimInstance->SetIsLive(false);
	}
}

void ANPCharacter::DeathEnd()
{
	if(m_CharacterState == EBehaviorState::Die)
	{

#pragma region DEBUG
		//if (GEngine) 
		//{
		//	GEngine->AddOnScreenDebugMessage
		//	(-1, 1.0f, FColor::Red, FString::FromInt(static_cast<int32>(m_fCurrentOpacity * 100.f)));
		//}
#pragma endregion

		if (m_fCurrentOpacity <= 0.f)
		{
			m_CharacterState = EBehaviorState::End;
			m_NextCharacterState = EBehaviorState::End;
			OnDeathEnd.Broadcast();
			GetWorld()->GetTimerManager().ClearTimer(m_hDeathTimer);

			// 메쉬가 가지고 있는 부착된 자식들을 모두 숨김 처리.
			TArray<USceneComponent*> AttachedActors = GetMesh()->GetAttachChildren();
			for (auto elem : AttachedActors)
			{
				elem->SetVisibleFlag(false);
				elem->SetHiddenInGame(true);
			}

			//this->SetActorEnableCollision(false);
			GetMesh()->SetHiddenInGame(true);
			m_HpBar->SetVisibility(false);
			return;
		}
		else
		{

			//DEBUG
			//UE_LOG(LogTemp, Log, TEXT("%s"), *(m_AnimInstance->Montage_GetCurrentSection().ToString()));


			// 이건 네비게이션 메시의 영향으로 지면아래로 이동이 안되는것 같다.
			//GetCharacterMovement()->AddForce(FVector{ 0.f,0.f,-10.f });
			//GetCharacterMovement()->MoveSmooth(FVector{ 0.f,0.f,-10.f }, 1.f);

			// 동적으로 머티리얼 인스턴스를 생성합니다.
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(m_OriginMaterial, this);
			// 투명도 값을 현재 값에서 0.1f만큼 감소시킵니다.
			if (nullptr != DynamicMaterial)
			{
				m_fCurrentOpacity -= 0.05f;
				DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), (FLinearColor::White * m_fCurrentOpacity));
				// 메시에 동적으로 생성된 머티리얼 인스턴스를 설정합니다.
				GetMesh()->SetMaterial(0, DynamicMaterial);
			}
			else
			{
				//DEBUG
				UE_LOG(LogTemp, Log, TEXT("DynamicMaterial : is nullptr"));
			}
		}


	} 
}

void ANPCharacter::OnHit()
{	
	m_CharacterState = EBehaviorState::Hit;
	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FName(FString::Printf(TEXT("Hit"))));

	// Effect
	Effect_Flick(FLinearColor::Red);
}

void ANPCharacter::OnDefense()
{
	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FName(FString::Printf(TEXT("Defense"))));

	m_CharacterState = EBehaviorState::Defense;
	m_NextCharacterState = EBehaviorState::Battle;
}

void ANPCharacter::StopDefense()
{
	m_AnimInstance->StopDeathMontage();
	


	m_CharacterState = m_NextCharacterState;
}

void ANPCharacter::Defense_Hit()
{
	m_CharacterState = EBehaviorState::Hit;

	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FName(FString::Printf(TEXT("Defense_Hit"))));

	// Effect
	Effect_Flick(FLinearColor::Black);
}

void ANPCharacter::OnDetect()
{
	if (m_CharacterState == EBehaviorState::Idle)
	{
		m_CharacterState = EBehaviorState::Detect;

		m_NextCharacterState = EBehaviorState::Alert;
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(TEXT("Detect"));
		
		// Effect
		Effect_Flick(FLinearColor::White);
	
	}

}

void ANPCharacter::OnAlert()
{

	if (FMath::IsNearlyEqual(m_AlertTime,-1.0) == true)
	{
		// 경계 상태의 대기시간을 지정하기 위해서 타이머 활용
		GetWorld()->GetTimerManager().SetTimer(m_hAlertTimer, this, &ANPCharacter::OnAlert, 1.0f, true);
		m_AlertTime = m_AlertCoolTime;
		
		// (임시) 이동속도 변경 .
		SetMoveSpeed((m_OriginMoveSpeed / 3.f));
	}
	else if (FMath::IsNearlyZero(m_AlertTime) == true)
	{
		// 전투 결정
		OnAlertEnd.Broadcast();
		OnBattle();

		m_AlertTime = -1.0;
		GetWorld()->GetTimerManager().ClearTimer(m_hAlertTimer);
	}
	else
	{
		if (FMath::RandRange(0, 9) < 2)
		{
			// 20% 확률로 시간 2초 단축 
			m_AlertTime += -2.0;
		}
		else
		{
			// 80% 확률로 시간 1초 단축
			m_AlertTime += -1.0;
		}
	}


}

void ANPCharacter::OnCommand(uint8 Command)
{
	FCommandData* CommandData = CastChecked<UMyGameInstance>(GetGameInstance())->GetCommandData(Command);

	auto Emotion = Cast<UEmotionWidget>(m_Emotion->GetUserWidgetObject());
	if (Emotion)
		Emotion->StartEmotion(CommandData->Icon,3.f);
}

bool ANPCharacter::IsOnTriggerEscape()
{
	// (임시) 0.2f 는 상수이니까 도망의 조건 값을 매개변수로 받거나  멤버 변수를 지정해서 세팅해줘야할듯하다. 
	return (m_pStatComp->GetHPRatio() <= 0.2f);
}

void ANPCharacter::OnStopAttackMontage(float InBlendOutTime)
{
	m_AnimInstance->StopAttackMontage(InBlendOutTime);
}

void ANPCharacter::OnStopDeathMontage(float InBlendOutTime)
{
	m_AnimInstance->StopDeathMontage(InBlendOutTime);
}

void ANPCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(m_CharacterState == EBehaviorState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_AnimInstance->StopAttackMontage();
		m_CharacterState = m_NextCharacterState;
		
	}
}

void ANPCharacter::OnDeathMontageEnded()
{
 
	
	//if (GEngine) {
	//	
	//	if (m_AnimInstance->GetMontageInst(1) == nullptr)
	//	{
	//		GEngine->AddOnScreenDebugMessage
	//		(-1, 10.0f, FColor::Red, TEXT("Montage Pointer : Nullptr"));
	//	}
	//	else
	//	{
	//		FString str(TEXT("<< Animation >> : "));
	//		str += m_AnimInstance->Montage_GetCurrentSection(m_AnimInstance->GetMontageInst(1)).ToString();
	//		GEngine->AddOnScreenDebugMessage
	//		(-1, 10.0f, FColor::Green, str);
	//		GEngine->AddOnScreenDebugMessage
	//		(-1, 10.0f, FColor::White, strState[m_CharacterState]);
	//	}
	//
	//}

	switch (m_CharacterState)
	{
		case EBehaviorState::Hit:
		{
			m_CharacterState = m_NextCharacterState;

			break;
		}
		case EBehaviorState::Defense:
		{
			m_CharacterState = m_NextCharacterState;
			OnDefenseEnd.Broadcast();

			break;
		}

		case EBehaviorState::Detect:
		{
			m_CharacterState = m_NextCharacterState;
			
			OnDetectEnd.Broadcast();
			break;
		}
		case EBehaviorState::Die:
		{
			this->SetActorEnableCollision(false);
			
			
			GetWorld()->GetTimerManager().SetTimer(m_hDeathTimer, this, &ANPCharacter::DeathEnd, 1.f, true);
			break;
		}
		case EBehaviorState::End:
		{

			break;
		}

		default :
		{
			
			break;
		}
	}


}

float ANPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_CharacterState == EBehaviorState::Die)
		return 0.f;

	// NextState
	m_NextCharacterState = EBehaviorState::Battle;
	m_Mode = ECharacterMode::Battle;

	// Overcome 극복하다. 10%의 확률로 회피 Dodge
	int32 Random = FMath::RandRange(0, 9);
	if (Random < 1)
	{
		// 저항하면 바로 전투 돌입.
		m_CharacterState = EBehaviorState::Battle;
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
			//m_CharacterState = EBehaviorState::Hit;	// Defense_Hit함수 안에다가 상태 전환을 넣었다.
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
			//m_CharacterState = EBehaviorState::Hit;	// Hit함수 안에다가 상태 전환을 넣었다.
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

	// DamageFont 
	FVector OverHead = ANPCharacter::GetMesh()->GetSocketLocation(TEXT("OverHead"));
	FVector2D OverHead2D;
	if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), OverHead, OverHead2D))
	{
		UUserWidget* DFW = CreateWidget(GetWorld(), m_DamageFont, TEXT(""));
		DFW->SetPositionInViewport(OverHead2D, true);
		DFW->AddToViewport();
		Cast<UDamageFontWidget>(DFW)->TakeDamage(DamageAmount);	// float -> int
	}


	return DamageAmount;

}

void ANPCharacter::SetupMonsterWidget(UMyUserWidget* InUserWidget)
{



}

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

void ANPCharacter::Debug()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, strState[m_CharacterState]);
}

