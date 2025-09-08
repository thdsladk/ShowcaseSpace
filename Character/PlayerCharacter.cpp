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

	// �±׸� �߰��ϱ�.
	Tags.Add(FName("PlayerCharacter"));

	// ĸ�� ������� �ʿ��ϸ� �ٽ� ����������.
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
	// �ӽ÷� ī�޶� �� ����
	m_CameraBoom->bInheritPitch = false;
	m_CameraBoom->bInheritYaw = false;
	m_CameraBoom->bInheritRoll = false;

	// �ʱ� ī�޶� �̵��ӵ� ����
	m_OriginCameraSpeed = m_CameraBoom->CameraLagSpeed;

	// Create a camera...
	m_TopDownCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	m_TopDownCameraComp->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	m_TopDownCameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Setup Components		// ������ �ڽĵ鿡�� �ٸ��� ����������.
	m_pStatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("StatComp"));



}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

#pragma region DEBUG
	// �浹üũ �Լ��� �����ϰ� ȣ���� Ÿ�̸� 
	//GetWorldTimerManager().SetTimer(m_Timer_ItemCollision,this, &APlayerCharacter::CheckForInteractable, 1.f, true, 1.f);
#pragma endregion

	AMyTest_TopDownPlayerController* PC = CastChecked<AMyTest_TopDownPlayerController>(GetController());
	if (PC != nullptr)
	{
		// HUDUpdate	���ε�
		PC->OnHUDUpdate.AddUObject(this, &APlayerCharacter::HUDUpdate);
		// HUD Init		�ʱ�ȭ
		auto Widget = Cast<UMyHUD>(PC->GetCurrentWidget());	// �����͸� ����Ʈ �����ͷ� ������ �ʿ䰡 �ִ�.
		if (Widget != nullptr)
		{
			Widget->Initialize();
			Widget->BindScreen1();
			Widget->BindStatus(m_pStatComp);
			Widget->BindSkill(m_pSkillComp);
		}

		EnableInput(PC);
	}

	// (�ӽ�) �浹 ä�� �������ֱ�.
	m_Ally = ECollisionChannel::ECC_GameTraceChannel4;
	m_Enemy = ECollisionChannel::ECC_GameTraceChannel2;

}

void APlayerCharacter::PostInitializeComponents()
{
	// ������Ʈ ���� �ʱ�ȭ�� ���� 
	Super::PostInitializeComponents();


	// �ִ� �ν��Ͻ� �ҷ����� 
	if (m_pAnimInstance != nullptr)
	{
		m_pAnimInstance->m_OnHitEnd.AddUObject(this, &APlayerCharacter::OnHitMontageEnded);
		m_pAnimInstance->m_OnDeathEnd.AddUObject(this, &APlayerCharacter::OnDeathMontageEnded);
	}
	else
	{

	}


	// ���ݿ��� OnDeathCheck�� Broadcast �ȴٸ� Death�Լ� ȣ�� 
	m_pStatComp->OnDeathCheck.AddUObject(this, &APlayerCharacter::Death);


	//	Combat Stat			
	/*
		StatComp�� Init ȣ�� ������ �÷��̾�ĳ������ �����ڶ� PostInitalizeComponents ���̶� ���� ��ġ�� �ξ���Ѵ�.
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

	/* ���� �Է� �ý��� */
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
	if (fabs(Value) > 0.00001f)	// ���������� ���ϱ�. ( float�� ���� ) 
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
		// �ӽ�
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
	// Ű�Է����� �����⵵ �ϰ� �ܺο��� ����� �����⵵ �Ѵ� �װ� �������.

	//AddMovementInput(GetActorForwardVector(), Value);
	GetCharacterMovement()->AddForce(GetActorForwardVector() * Value);

	// �ִϸ��̼� �� ���� ���� �̶� �̵� �Ŀ� �Ѵ�.
	m_Vertical = FMath::Clamp(Value, -1.f, 1.f);

}

void APlayerCharacter::MoveToRight(float Value)
{
	// Ű�Է����� �����⵵ �ϰ� �ܺο��� ����� �����⵵ �Ѵ� �װ� �������.

	//AddMovementInput(GetActorRightVector(), Value);
	GetCharacterMovement()->AddForce(GetActorRightVector() * Value);

	// �ִϸ��̼� �� ���� ���� �̶� �̵� �Ŀ� �Ѵ�.
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);
}

void APlayerCharacter::Sprint(float WalkSpeedRatio, float CameraSpeedRatio, bool HasState)
{
	// ���°� ���Ե� ������� ����
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

	GetCharacterMovement()->MaxWalkSpeed = (m_OriginMoveSpeed * WalkSpeedRatio);									// ĳ���� �̵��ӵ�	
	m_CameraBoom->CameraLagSpeed = (m_OriginCameraSpeed * CameraSpeedRatio);	// �̵��ӵ� 2.0f �϶� 8.0f			// ī�޶� �����ӵ�

	// (�ӽ�) ������ ī����
	GetWorld()->GetTimerManager().SetTimer(m_hGaugeTimer,
		[this]() { this->m_pStatComp->AddSP(-1); }
	, 0.1f, true);

}

void APlayerCharacter::SetSprint(float WalkSpeed, float CameraSpeed, bool HasState)
{
	// ���°� ���Ե� ������� ����
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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// ĳ���� �̵��ӵ�
	m_CameraBoom->CameraLagSpeed = CameraSpeed;				// ī�޶� �����ӵ�
}

void APlayerCharacter::StopSprint(bool HasState)
{
	// ���°� ���Ե� ������� ����
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
			// �߸��� ���� �����̴�.
			UE_LOG(LogTemp, Log, TEXT("Invalid input state detected."));

			return;
		}
	}


	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;		// ĳ���� �̵��ӵ�
	m_CameraBoom->CameraLagSpeed = m_OriginCameraSpeed;				// ī�޶� �����ӵ�

	// (�ӽ�) ������ ī����
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
	// To Do   ���⿡�� �״� �ִϸ��̼��� ������ ���� ��Ȳ�� ó���� ����. 

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


		// �ӽÿ� ���� ���� 
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
		// ������ ä���.
		AMyItem* Item = Cast<AMyItem>(m_CurrentInteractable);
		if (Item != nullptr)
		{
			FItemRuntimeData ItemRuntimeData;
			Item->PopItemData(ItemRuntimeData);
			if (ACharacterBase::m_pInventoryComp->TakeItem(ItemRuntimeData) == false)
			{
				// ������ ��� ����

			}
		}

		if (m_CurrentInteractable != nullptr)
		{
			m_CurrentInteractable->Interact_Implementation();
			// ����� ��ȣ�ۿ� ��ü�� ������¿��� �����ֱ� [ �������� ��� ������ ���� ��ȣ�ۿ��� �������� ]
			m_CurrentInteractable = nullptr;
		}
	}
}

void APlayerCharacter::CheckForInteractable()
{
	// Sphere Collision ���� �浹�ؼ� �к��ϱ� 

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
	// �ϴ��� ��ų���õ� ��ư�鸸 �����ϰ� ���߿� Ȯ������. 
// GlobalEnum���� ButtonEnum�� �ϳ� ���� ����� ���� �ִ�.

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
		// ���⼭ ���ʹ� �ý��� ���� Ű�� ������ ĳ���� ���� Ű��
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
	// ���� ���ȿ��� �ӵ��� �Ȱ��������ִ�.
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
	// ���⼭ ���ʹ� �ý��� ���� Ű�� ������ ĳ���� ���� Ű��
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
	// (�ӽ�) ������ �ݰ� ����
	if (m_CharacterState == EBehaviorState::Defense)
	{
		if (m_pStatComp->GetSP() >= CombatInfo.CostDefense_SP)
		{
			// (�ӽ�) ������ �޴� Hit ���� ����
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
