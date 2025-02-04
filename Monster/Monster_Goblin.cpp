// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Goblin.h"
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

#include "NavigationSystem.h"

// Sets default values
AMonster_Goblin::AMonster_Goblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Skeletal Mesh
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin.SK_Goblin'"));
	//
	//if (SM.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SM.Object);
	//}

	// 임시 Stat
	m_pStatComp = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("Stat"));


	// HP bar
	m_HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	m_HpBar->SetupAttachment(GetMesh());
	m_HpBar->SetRelativeLocation(FVector{ 0.f,0.f,200.f });
	m_HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TopDown/UI/WBP_HPBar.WBP_HPBar_C'"));
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

	static ConstructorHelpers::FObjectFinder<UMaterial> Shiny(TEXT("/Script/Engine.Material'/Game/TopDown/Material/M_Shiny.M_Shiny'"));
	if (Shiny.Succeeded())
	{
		m_Shiny = Shiny.Object;
	}


	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
// Called when the game starts or when spawned
void AMonster_Goblin::BeginPlay()
{
	Super::BeginPlay();
	
	// OriginMaterial Init Setting
	m_OriginMaterial = GetMesh()->GetMaterial(0);


	m_MonsterType = FName("Goblin_Red-1");
	FMonsterData* MonsterData = Cast<UMyGameInstance>(GetGameInstance())->GetMonsterStatData(m_MonsterType.ToString());
	//// DuplicateObject를 통해서 생성한 메시객체는 어디서 메모리 관리를 할까 직접 멤버관리를 하는것일까? 
	//// UObject의 자식이라서 일단 가비지 컬렉터가 관리할것으로 보인다. 
	//GetMesh()->SetSkeletalMesh(DuplicateObject<USkeletalMesh>(MonsterData->SkeletalMesh, this));

	//AttachmentEquipment Weapon1

	FName WeaponSocket(TEXT("Weapon_Socket"));
	m_Equipment1 = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());

	if (nullptr != m_Equipment1)
	{
		m_Equipment1->SetItem(MonsterData->LItemID);
		m_Equipment1->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
		m_Equipment1->OnEquip();

		m_Equipment1->FinishSpawning(FTransform());
	}

	//AttachmentEquipment Weapon2

	FName ShieldSocket(TEXT("Shield_Socket"));
	m_Equipment2 = GetWorld()->SpawnActorDeferred<AMyEquipment>(AMyEquipment::StaticClass(), FTransform());

	if (nullptr != m_Equipment2)
	{
		m_Equipment2->SetItem(MonsterData->RItemID);
		m_Equipment2->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ShieldSocket);
		m_Equipment2->OnEquip();

		m_Equipment2->FinishSpawning(FTransform());
	}


	m_AnimInstance = Cast<UAnimInstance_Goblin>(GetMesh()->GetAnimInstance());
	if (m_AnimInstance == nullptr)
		return;

	//AnimInstance->OnMontageEnded.AddDynamic(this, &AMonster_Goblin::OnAttackMontageEnded);

	m_AnimInstance->OnAttackHit.AddUObject(this, &AMonster_Goblin::AttackCheck);
	m_AnimInstance->OnAttackEnd.AddUObject(this, &AMonster_Goblin::AttackEnd);

	m_AnimInstance->OnDeathPoint.AddUObject(this, &AMonster_Goblin::OnDeathMontageEnded);

	// 스텟에서 OnDeathCheck가 Broadcast 된다면 Death함수 호출 
	m_pStatComp->OnDeathCheck.AddUObject(this, &AMonster_Goblin::Death);

	//m_pStatComp->SetLevel(m_pStatComp->GetLevel()); // 스텟 초기화 


	//DEBUG 
	GetWorld()->GetTimerManager().SetTimer(m_hDebug, this, &AMonster_Goblin::Debug, 0.1f, true);

	


}
// Called every frame
void AMonster_Goblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Debug

	//UE_LOG(LogTemp, Log, TEXT("MonState : %d"),m_State);
}
// Called to bind functionality to input
void AMonster_Goblin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);



}

void AMonster_Goblin::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	m_HpBar->InitWidget();
	m_Emotion->InitWidget();

	// TODO
	auto HpWidget = Cast<UPawnWidget>(m_HpBar->GetUserWidgetObject());
	if (HpWidget)
		HpWidget->BindHp(m_pStatComp);


}

void AMonster_Goblin::Attack()
{
	m_AnimInstance->PlayAttackMontage();
	m_AnimInstance->JumpToSection(m_AttackIndex);
	m_AttackIndex = (m_AttackIndex + 1) % 3; // Attack 모션이 2개 이므로

	m_State = EState::Attacking;
	m_NextState = EState::Battle;
}

void AMonster_Goblin::AttackCheck()
{
	if (m_State != EState::Attacking)
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

void AMonster_Goblin::Death()
{
	if (m_State < EState::Die)
	{
		m_State = EState::Die;
		m_NextState = EState::Die;
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(TEXT("Death_B"));

		m_AnimInstance->SetIsLive(false);
	}
}

void AMonster_Goblin::Death_End()
{
	if(m_State == EState::Die)
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
			m_State = EState::End;
			m_NextState = EState::End;
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

void AMonster_Goblin::Attacked()
{	
	if (m_State == EState::Hit)
	{
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(FString::Printf(TEXT("Hit")));

		// Effect
		Effect_Flick(FLinearColor::Red);
	}

}

void AMonster_Goblin::AttackEnd()
{
	if (m_State == EState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_AnimInstance->StopMontage(0);
		m_State = m_NextState;
	}
}

void AMonster_Goblin::Defense_Hit()
{
	m_AnimInstance->PlayDeathMontage();
	m_AnimInstance->JumpToSection_Death(FString::Printf(TEXT("Defense_Hit")));

	// Effect
	Effect_Flick(FLinearColor::Black);
}

void AMonster_Goblin::OnDetect()
{
	if (m_State == EState::Idle)
	{
		m_State = EState::Detect;

		m_NextState = EState::Alert;
		m_AnimInstance->PlayDeathMontage();
		m_AnimInstance->JumpToSection_Death(TEXT("Detect"));
		
		// Effect
		Effect_Flick(FLinearColor::White);
	


		// 임시 Emotion 
		auto Emotion = Cast<UEmotionWidget>(m_Emotion->GetUserWidgetObject());
		if (Emotion)
			Emotion->StartEmotion(3.f);
	}

}

void AMonster_Goblin::OnAlert()
{
	// 경계 상태의 대기시간을 지정하기 위해서 타이머 활용
	GetWorld()->GetTimerManager().SetTimer(m_hAlertTimer, this, &AMonster_Goblin::OnAlert, 2.0f, false);

	// 경계 상태 다음 행동을 결정 해준다. 
	if (FMath::RandRange(0, 9) < 5)
	{
		// 전투 결정
		OnAlertEnd.Broadcast();
		m_State = EState::Battle;
		m_NextState = EState::Battle; 

		GetWorld()->GetTimerManager().ClearTimer(m_hAlertTimer);
	}
	else
	{
		// 
	}
}

void AMonster_Goblin::Effect_Flick(FLinearColor Color)
{
	if (true == GetWorld()->GetTimerManager().IsTimerActive(m_hEffectFlickTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer(m_hEffectFlickTimer, this, &AMonster_Goblin::EffectEnd_Flick, 0.1f, false);
	
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(m_Shiny, this);
	DynamicMaterial->SetVectorParameterValue(FName("Param_1"), Color);

	GetMesh()->SetMaterial(0, DynamicMaterial);

	//DEBUG
	
	//bool OK = false;
	//if (DynamicMaterial->IsVectorParameterUsedAsChannelMask(TEXT("Param_1"), OK) == true)
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : OK ")); }
	//else 
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : Failed ")); }
}

void AMonster_Goblin::EffectEnd_Flick()
{
	GetMesh()->SetMaterial(0, m_OriginMaterial);
}

void AMonster_Goblin::StopMontage(int32 iState)
{
	m_AnimInstance->StopMontage(iState);
}

void AMonster_Goblin::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(m_State == EState::Attacking)
	{
		OnAttackEnd.Broadcast();
		m_AnimInstance->StopMontage(0);
		m_State = m_NextState;
	}
}

void AMonster_Goblin::OnDeathMontageEnded()
{
	// 여기서 몬스터를 파괴한다 .
	// To DO  
	
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
	//		(-1, 10.0f, FColor::White, strState[m_State]);
	//	}
	//
	//}

	switch (m_State)
	{
		case EState::Hit:
		{
			OnHitEnd.Broadcast();
			m_State = m_NextState;
			
			break;
		}
		case EState::Defense:
		{
			OnDefenseEnd.Broadcast();
			m_State = m_NextState;

			break;
		}

		case EState::Detect:
		{
			OnDetectEnd.Broadcast();
			m_State = m_NextState;
			break;
		}
		case EState::Die:
		{
			this->SetActorEnableCollision(false);
			
			
			GetWorld()->GetTimerManager().SetTimer(m_hDeathTimer, this, &AMonster_Goblin::Death_End, 1.f, true);
			break;
		}
		case EState::End:
		{

			break;
		}

		default :
		{
			
			break;
		}
	}


}

void AMonster_Goblin::MoveToFoward(float Value)
{
	m_Vertical = Value;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMonster_Goblin::MoveToSide(float Value)
{
	m_Horizontal = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

void AMonster_Goblin::LookDirection(float Value)
{
	 m_Horizontal = Value;
	 //m_Vertical = -Value;
	 //Debug
	 UE_LOG(LogTemp, Log, TEXT("Horizontal : %f "), m_Horizontal);
}

void AMonster_Goblin::SetMoveSpeed(float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// 캐릭터 이동속도
}

float AMonster_Goblin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_State == EState::Die)
		return 0.f;

	// NextState
	m_NextState = EState::Battle;

	// Overcome 극복하다. 10%의 확률로 회피 Dodge
	int32 Random = FMath::RandRange(0, 9);
	if (Random < 1)
	{
		// 저항하면 바로 전투 돌입.
		m_State = EState::Battle;
		DamageAmount = 0;
	}
	else if(Random < 5)
	{
		// 방어 상태. 
		m_State = EState::Defense;
		DamageAmount *= 0.5f;
	}
	else 
	{
		// Change State
		m_State = EState::Hit;
		m_pStatComp->OnHPChanged.AddLambda([this,DamageAmount]()
			{
				if (DamageAmount > 0)
				{
					this->Attacked();
				}
			});
	}

	// Set Stat
	m_pStatComp->OnAttacked(DamageAmount);

	// DamageFont 
	FVector OverHead = AMonster_Goblin::GetMesh()->GetSocketLocation(TEXT("OverHead"));
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

void AMonster_Goblin::SetupMonsterWidget(UMyUserWidget* InUserWidget)
{



}

void AMonster_Goblin::Debug()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, strState[m_State]);
}

