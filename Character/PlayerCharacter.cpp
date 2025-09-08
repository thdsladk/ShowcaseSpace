// Fill out your copyright notice in the Description page of Project Settings.

// Default Section
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sub Section
#include "MyGameInstance.h"
#include "MyTest_TopDownGameMode.h"
#include "MyTest_TopDownPlayerController.h"
#include "Animation/AnimInstanceBase.h"

// Widget Section
#include "MyHUD.h"
#include "MyInventoryWidget.h"
#include "StatWidget.h"

// Component Section
#include "CharacterComponent/MyStatComponent.h"
#include "CharacterComponent/MyInventoryComponent.h"
#include "CharacterComponent/SkillComponent.h"
#include "AbilityComponent.h"
#include "Components/AudioComponent.h"

// etc Section
#include "Item/MyItem.h"
#include "Utill/RandomSystem.h"
#include "DrawDebugHelpers.h"

// Audio Section
#include "Sound/SoundCue.h"


APlayerCharacter::APlayerCharacter()
	:m_bShoulderView(false)
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// 태그를 추가하기.
	Tags.Add(FName("PlayerCharacter"));

	// 캡슐 사이즈는 필요하면 다시 설정해주자.
	//// Set size for player capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Create a camera boom...
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(RootComponent);
	m_CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	m_CameraBoom->TargetArmLength = 2800.f;
	m_CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	m_CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	// 임시로 카메라 암 설정
	m_CameraBoom->bInheritPitch = false;
	m_CameraBoom->bInheritYaw = false;
	m_CameraBoom->bInheritRoll = false;

	// 초기 카메라 이동속도 보관
	m_OriginCameraSpeed = m_CameraBoom->CameraLagSpeed;

	// Create a camera...
	m_TopDownCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	m_TopDownCameraComp->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	m_TopDownCameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Setup Components		// 스탯은 자식들에서 다르게 세팅해주자.
	m_pStatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("StatComp"));



}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

#pragma region DEBUG
	// 충돌체크 함수를 일정하게 호출할 타이머 
	//GetWorldTimerManager().SetTimer(m_Timer_ItemCollision,this, &APlayerCharacter::CheckForInteractable, 1.f, true, 1.f);
#pragma endregion

	AMyTest_TopDownPlayerController* PC = CastChecked<AMyTest_TopDownPlayerController>(GetController());
	if (PC != nullptr)
	{
		// HUDUpdate	바인딩
		PC->OnHUDUpdate.AddUObject(this, &APlayerCharacter::HUDUpdate);
		// HUD Init		초기화
		auto Widget = Cast<UMyHUD>(PC->GetCurrentWidget());	// 포인터를 스마트 포인터로 관리할 필요가 있다.
		if (Widget != nullptr)
		{
			Widget->Initialize();
			Widget->BindScreen1();
			Widget->BindStatus(m_pStatComp);
			Widget->BindSkill(m_pSkillComp);
		}

		EnableInput(PC);
	}

	// (임시) 충돌 채널 설정해주기.
	m_Ally = ECollisionChannel::ECC_GameTraceChannel4;
	m_Enemy = ECollisionChannel::ECC_GameTraceChannel2;

}

void APlayerCharacter::PostInitializeComponents()
{
	// 컴포넌트 들이 초기화된 이후 
	Super::PostInitializeComponents();


	// 애님 인스턴스 불러오기 
	if (m_pAnimInstance != nullptr)
	{
		m_pAnimInstance->m_OnHitEnd.AddUObject(this, &APlayerCharacter::OnHitMontageEnded);
		m_pAnimInstance->m_OnDeathEnd.AddUObject(this, &APlayerCharacter::OnDeathMontageEnded);
	}
	else
	{

	}


	// 스텟에서 OnDeathCheck가 Broadcast 된다면 Death함수 호출 
	m_pStatComp->OnDeathCheck.AddUObject(this, &APlayerCharacter::Death);


	//	Combat Stat			
	/*
		StatComp의 Init 호출 시점이 플레이어캐릭터의 생성자랑 PostInitalizeComponents 사이라서 현재 위치에 두어야한다.
	*/
	m_AttackRange = m_pStatComp->GetTotalStat().AttackRange;
	m_AttackRadius = m_pStatComp->GetTotalStat().AttackRadius;
	m_RangedRadius = m_AttackRadius + m_AttackRange;
	m_DetectionRadius = m_RangedRadius * 4.f;
	m_VisibleRadius = m_DetectionRadius * 2.f;

}

void APlayerCharacter::SetupCharacterData()
{
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* 기존 입력 시스템 */
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::Look_UpDown);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::Look_LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Wheel"), this, &APlayerCharacter::Wheel_UpDown);
	PlayerInputComponent->BindAxis(TEXT("Vertical_Keyboard"), this, &APlayerCharacter::MoveToForward);
	PlayerInputComponent->BindAxis(TEXT("Horizontal_Keyboard"), this, &APlayerCharacter::MoveToRight);
}

void APlayerCharacter::ChangeCameraView()
{
	if (m_bShoulderView == true)
	{
		bUseControllerRotationYaw = false;
		m_CameraBoom->TargetArmLength = 2800.f;
		m_CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
		m_CameraBoom->bUsePawnControlRotation = false;
		m_bShoulderView = false;
	}
	else
	{
		bUseControllerRotationYaw = true;
		m_CameraBoom->TargetArmLength = 800.f;
		m_CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		m_CameraBoom->bUsePawnControlRotation = true;
		m_bShoulderView = true;
	}

}

void APlayerCharacter::OnDefense()
{
	SetState(EBehaviorState::Defense);
	SetNextState(EBehaviorState::Battle);
}

void APlayerCharacter::StopDefense()
{
	SetState(m_NextCharacterState);
}

void APlayerCharacter::Death()
{
	if (m_CharacterState != EBehaviorState::Die)
	{
		Super::Death();
	}
}

void APlayerCharacter::Look_UpDown(float Value)
{
	if (fabs(Value) > 0.00001f)	// 오차범위로 비교하기. ( float을 비교중 ) 
	{
		if (m_bShoulderView == true)
		{
			//AddControllerPitchInput(Value * 100.f);
			//CastChecked<APlayerController>(GetController())->AddPitchInput(Value * 100.f);
		}
	}
}

void APlayerCharacter::Look_LeftRight(float Value)
{
	if (m_bShoulderView == true)
	{
		// 임시
		int32 ViewportSizeX;
		int32 ViewportSizeY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(ViewportSizeX, ViewportSizeY);

		FVector2D CenterOfScreen = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);

		float MouseX, MouseY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MouseX, MouseY);

		FVector2D MouseMovement = FVector2D(MouseX, MouseY) - CenterOfScreen;


		//CameraBoom->AddRelativeRotation(FRotator(0.f, MouseMovement.X *0.001, 0.f));
		CastChecked<APlayerController>(GetController())->AddYawInput(MouseMovement.X * 0.001f);

		//this->AddActorWorldRotation(FRotator(0.f, MouseMovement.X * 0.001f, 0.f));
	}
}

void APlayerCharacter::Wheel_UpDown(float Value)
{
	if (FMath::IsNearlyZero(Value) == true)return;

	if (m_bShoulderView == true)
	{

	}
	else
	{
		m_CameraBoom->TargetArmLength += (Value * 10.f);
	}
}

#pragma region Movement Function
void APlayerCharacter::MoveToForward(float Value)
{
	// 키입력으로 들어오기도 하고 외부에서 제어로 들어오기도 한다 그걸 고려하자.

	//AddMovementInput(GetActorForwardVector(), Value);
	GetCharacterMovement()->AddForce(GetActorForwardVector() * Value);

	// 애니메이션 을 위한 보간 이라서 이동 후에 한다.
	m_Vertical = FMath::Clamp(Value, -1.f, 1.f);

}

void APlayerCharacter::MoveToRight(float Value)
{
	// 키입력으로 들어오기도 하고 외부에서 제어로 들어오기도 한다 그걸 고려하자.

	//AddMovementInput(GetActorRightVector(), Value);
	GetCharacterMovement()->AddForce(GetActorRightVector() * Value);

	// 애니메이션 을 위한 보간 이라서 이동 후에 한다.
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);
}

void APlayerCharacter::Sprint(float WalkSpeedRatio, float CameraSpeedRatio, bool HasState)
{
	// 상태가 포함된 기능인지 구분
	if (HasState == true)
	{
		if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
		{
			SetState(EBehaviorState::Running);
			m_pAnimInstance->SetIsSprint(true);
		}
		else
		{
			return;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = (m_OriginMoveSpeed * WalkSpeedRatio);									// 캐릭터 이동속도	
	m_CameraBoom->CameraLagSpeed = (m_OriginCameraSpeed * CameraSpeedRatio);	// 이동속도 2.0f 일때 8.0f			// 카메라 지연속도

	// (임시) 게이지 카운터
	GetWorld()->GetTimerManager().SetTimer(m_hGaugeTimer,
		[this]() { this->m_pStatComp->AddSP(-1); }
	, 0.1f, true);

}

void APlayerCharacter::SetSprint(float WalkSpeed, float CameraSpeed, bool HasState)
{
	// 상태가 포함된 기능인지 구분
	if (HasState == true)
	{
		if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
		{
			SetState(EBehaviorState::Running);
			m_pAnimInstance->SetIsSprint(true);
		}
		else
		{
			return;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// 캐릭터 이동속도
	m_CameraBoom->CameraLagSpeed = CameraSpeed;				// 카메라 지연속도
}

void APlayerCharacter::StopSprint(bool HasState)
{
	// 상태가 포함된 기능인지 구분
	if (HasState == true)
	{
		if (m_Mode == ECharacterMode::Idle)
		{
			SetState(EBehaviorState::Idle);
			m_pAnimInstance->SetIsSprint(false);
		}
		else if (m_Mode == ECharacterMode::Battle)
		{
			SetState(EBehaviorState::Battle);
			m_pAnimInstance->SetIsSprint(false);
		}
		else
		{
			// 잘못된 상태 정지이다.
			UE_LOG(LogTemp, Log, TEXT("Invalid input state detected."));

			return;
		}
	}


	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;		// 캐릭터 이동속도
	m_CameraBoom->CameraLagSpeed = m_OriginCameraSpeed;				// 카메라 지연속도

	// (임시) 게이지 카운터
	GetWorld()->GetTimerManager().ClearTimer(m_hGaugeTimer);
}
#pragma endregion

void APlayerCharacter::HUDUpdate(uint8 Type)
{
	AMyTest_TopDownPlayerController* PC = CastChecked<AMyTest_TopDownPlayerController>(GetController());

	// TODO
	switch (PC->GetHUDState())
	{
	case AMyTest_TopDownPlayerController::EHUDState::EIngame:
	{
		auto Widget = Cast<UMyHUD>(PC->GetCurrentWidget());
		if (Widget != nullptr)
		{
			Widget->BindStatus(m_pStatComp);
		}
		break;
	}
	case AMyTest_TopDownPlayerController::EHUDState::EInventory:
	{
		auto Widget = Cast<UMyInventoryWidget>(PC->GetCurrentWidget());
		if (Widget != nullptr)
		{
			Widget->BindInventory(*m_pInventoryComp);
			Widget->UpdateWidget();
		}
		break;
	}
	case AMyTest_TopDownPlayerController::EHUDState::EShop:
	{
		break;
	}
	case AMyTest_TopDownPlayerController::EHUDState::EStatus:
	{
		auto Widget = Cast<UStatWidget>(PC->GetCurrentWidget());
		if (Widget != nullptr)
		{
			Widget->BindStat(*m_pStatComp);
		}
		break;
	}
	case AMyTest_TopDownPlayerController::EHUDState::ESkill:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

#pragma region Animation Function
void APlayerCharacter::OnDeathMontageEnded()
{
	// To Do   여기에서 죽는 애니메이션이 끝나고 나서 상황을 처리해 주자. 

	switch (m_CharacterState)
	{
	case EBehaviorState::Die:
	{
		AMyTest_TopDownPlayerController* PC = Cast<AMyTest_TopDownPlayerController>(GetController());
		if (nullptr != PC)
		{
			DisableInput(PC);

			IGameInterface* GameInterface = Cast<IGameInterface>(GetWorld()->GetAuthGameMode());
			if (GameInterface != nullptr)
			{
				GameInterface->OnPlayerDead();
			}
		}


		// 임시용 게임 종료 
		//UWorld* CurWorld = GetWorld();
		//UKismetSystemLibrary::QuitGame(CurWorld, CurWorld->GetFirstPlayerController()
		//	, EQuitPreference::Quit, false);

		break;

	}
	default:
	{
		break;
	}
	}

}

void APlayerCharacter::OnHitMontageEnded()
{
	if (m_CharacterState == EBehaviorState::Hit)
	{
		AMyTest_TopDownPlayerController* PC = CastChecked<AMyTest_TopDownPlayerController>(GetController());
		PC->OnGamePlayControlReturned();

		SetState(m_NextCharacterState);
	}
}
#pragma endregion

#pragma region Interact Interface
void APlayerCharacter::ApplyInteraction()
{
	//CheckForInteractable();

	if (m_CurrentInteractable != nullptr)
	{
		// 아이템 채우기.
		AMyItem* Item = Cast<AMyItem>(m_CurrentInteractable);
		if (Item != nullptr)
		{
			FItemRuntimeData ItemRuntimeData;
			Item->PopItemData(ItemRuntimeData);
			if (ACharacterBase::m_pInventoryComp->TakeItem(ItemRuntimeData) == false)
			{
				// 아이템 얻기 실패

			}
		}

		if (m_CurrentInteractable != nullptr)
		{
			m_CurrentInteractable->Interact_Implementation();
			// 사용한 상호작용 객체는 현재상태에서 지워주기 [ 아이템의 경우 쓰고나면 현재 상호작용을 할이유가 ]
			m_CurrentInteractable = nullptr;
		}
	}
}

void APlayerCharacter::CheckForInteractable()
{
	// Sphere Collision 으로 충돌해서 분별하기 

	FHitResult HitResult;

	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(120.f);
	FCollisionQueryParams CQP(TEXT("InteractableCollision"), true, this);

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECC_GameTraceChannel3/*ECC_WorldDynamic*/, ExplosionSphere, CQP);

	if (bHit == true)
	{
		m_CurrentInteractable = Cast<AMyInteractable>(HitResult.GetActor());
		if (m_CurrentInteractable == nullptr)
		{
			m_HelpText = TEXT("");
			m_CurrentInteractable = nullptr;
		}
		else
		{
			m_HelpText = m_CurrentInteractable->GetHelpText();
		}

		// Debug
		//	DrawDebugSphere(GetWorld(), GetActorLocation(), 120.f, 16.f, FColor::Green, false, 0.1f, 0, 1.0f);
	}
	else
	{
		// Debug
			//	DrawDebugSphere(GetWorld(), GetActorLocation(), 120.f, 16.f, FColor::Red, false, 0.1f, 0, 1.0f);
	}
}

void APlayerCharacter::SetCurrentInteractable(AMyInteractable* Interactable)
{
	m_CurrentInteractable = Interactable;
}
#pragma endregion

#pragma region Controller Section
void APlayerCharacter::ClickButton(uint8 Button)
{
	// 일단은 스킬관련된 버튼들만 구현하고 나중에 확장하자. 
// GlobalEnum에서 ButtonEnum을 하나 만들어서 써야할 수도 있다.

	switch (Button)
	{
		case AMyTest_TopDownPlayerController::EKey::Key_Q:
		{
			//m_pSkillComp->UseSkill(Button);
			Click_Q();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_E:
		{
			Click_E();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_R:
		{
			Click_R();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_W:
		{
			Click_W();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_V:
		{
			ChangeCameraView();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_A:
		{

			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_S:
		{
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_D:
		{
			OnDefense();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_G:
		{

			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_H:
		{

			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_J:
		{

			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_T:
		{

			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_Space:
		{
			//Jump();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_Shift:
		{
			Sprint();
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_F:
		{
			ApplyInteraction();
			break;
		}
		// 여기서 부터는 시스템 단위 키들 위에는 캐릭터 단위 키들
		case AMyTest_TopDownPlayerController::EKey::Key_Tab:
		{
			break;
		}
		case AMyTest_TopDownPlayerController::EKey::Key_P:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void APlayerCharacter::Click_F()
{
	ApplyInteraction();
}

void APlayerCharacter::Click_Q()
{
	m_pSkillComp->UseSkill(static_cast<uint8>(ESkill::Skill_Q));
}

void APlayerCharacter::Click_W()
{
	//m_pSkillComp->UseSkill(static_cast<uint8>(ESkill::Skill_W));
	FName Tag(TEXT("Ability.Skill.StormKill"));

	FAbilityHandle Handle;
	FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(Tag);
	m_pAbilityComp->TryActivateByTag(GameplayTag, Handle);
}

void APlayerCharacter::Click_E()
{
	//m_pSkillComp->UseSkill(static_cast<uint8>(ESkill::Skill_E));
	FName Tag(TEXT("Ability.Skill.FireBall"));

	FAbilityHandle Handle;
	FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(Tag);
	m_pAbilityComp->TryActivateByTag(GameplayTag, Handle);
}

void APlayerCharacter::Click_R()
{
	//m_pSkillComp->UseSkill(static_cast<uint8>(ESkill::Skill_R));
	FName Tag(TEXT("Ability.Skill.WaterBall"));
	
	FAbilityHandle Handle;
	FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(Tag);
	m_pAbilityComp->TryActivateByTag(GameplayTag, Handle);
}

void APlayerCharacter::Click_V()
{
	ChangeCameraView();
}

void APlayerCharacter::Click_A()
{
}

void APlayerCharacter::Click_S()
{
}

void APlayerCharacter::Click_D()
{
	OnDefense();
}

void APlayerCharacter::Click_G()
{
}

void APlayerCharacter::Click_H()
{
}

void APlayerCharacter::Click_J()
{
}

void APlayerCharacter::Click_T()
{
}

void APlayerCharacter::Click_Space()
{
	//ACharacter::Jump();
}

void APlayerCharacter::Click_Shift()
{
	// 아직 스탯에서 속도를 안가져오고있다.
	Sprint();
}

void APlayerCharacter::ReleaseButton(uint8 Button)
{
	switch (Button)
	{
	case AMyTest_TopDownPlayerController::EKey::Key_Q:
	case AMyTest_TopDownPlayerController::EKey::Key_W:
	case AMyTest_TopDownPlayerController::EKey::Key_E:
	case AMyTest_TopDownPlayerController::EKey::Key_R:
	{
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_V:
	{
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_A:
	{

		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_S:
	{
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_D:
	{
		StopDefense();
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_Space:
	{
		Attack();
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_Shift:
	{
		StopSprint();
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_F:
	{
		break;
	}
	// 여기서 부터는 시스템 단위 키들 위에는 캐릭터 단위 키들
	case AMyTest_TopDownPlayerController::EKey::Key_Tab:
	{
		break;
	}
	case AMyTest_TopDownPlayerController::EKey::Key_P:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

void APlayerCharacter::Release_Space()
{
	//ACharacter::StopJumping();

	Attack();
}

void APlayerCharacter::Release_Shift()
{
	StopSprint();
}

void APlayerCharacter::ClickRMouse()
{
}
#pragma endregion

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (m_bTakeDamage == false || m_pStatComp->GetHP() == 0)
	{
		return 0.f;
	}
	
	FCombatInfo CombatInfo;
	// (임시) 데미지 반감 로직
	if (m_CharacterState == EBehaviorState::Defense)
	{
		if (m_pStatComp->GetSP() >= CombatInfo.CostDefense_SP)
		{
			// (임시) 데미지 받는 Hit 상태 생략
			//OnHit();
			m_pStatComp->AddSP(-CombatInfo.CostDefense_SP);
			DamageAmount *= 0.5f;
			m_pStatComp->OnAttacked(DamageAmount);
		}
		else
		{
			OnHit();
			m_pStatComp->OnAttacked(DamageAmount);
		}
	}
	else
	{
		OnHit();
		m_pStatComp->OnAttacked(DamageAmount);
	}

	// Set Aggro Gauge 
	m_AggroGauge = FMath::Clamp(m_AggroGauge + CombatInfo.TakeDamage_Aggro, 0, 100);

	return DamageAmount;
}

void APlayerCharacter::ApplyHidden(const bool bHide)
{
	Super::ApplyHidden(bHide);
}
