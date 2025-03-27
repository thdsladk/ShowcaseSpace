// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest_TopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyAIController.h"
#include "MyTest_TopDownPlayerController.h"
#include "Engine/DamageEvents.h"
#include "MyTest_TopDownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

// Widget Section
#include "MyHUD.h"
#include "MyInventoryWidget.h"
#include "StatWidget.h"

// Component Section
#include "MyStatComponent.h"
#include "MyInventoryComponent.h"
#include "SkillComponent.h"
#include "CharacterComponent/MyEquipmentComponent.h"

// etc Section
#include "MyItem.h"
#include "MyInteractable.h"
#include "Utill/RandomSystem.h"


AMyTest_TopDownCharacter::AMyTest_TopDownCharacter()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

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

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm




	// 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Meshes/Grux.Grux"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

 	m_pStatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("Stat"));
	m_pInventoryComp = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("Inventory"));
	m_pSkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("Skill"));
	m_pEquipmentComp = CreateDefaultSubobject<UMyEquipmentComponent>(TEXT("Equip"));





}

void AMyTest_TopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �浹üũ �Լ��� �����ϰ� ȣ���� Ÿ�̸� 
	//GetWorldTimerManager().SetTimer(m_Timer_ItemCollision,this, &AMyTest_TopDownCharacter::CheckForInteractable, 1.f, true, 1.f);

	AMyTest_TopDownPlayerController* PC = CastChecked<AMyTest_TopDownPlayerController>(GetController());
	if (nullptr != PC)
	{
		// HUDUpdate	���ε�
		PC->OnHUDUpdate.AddUObject(this, &AMyTest_TopDownCharacter::HUDUpdate);
		// HUD Init		�ʱ�ȭ
		auto Widget = Cast<UMyHUD>(PC->GetCurrentWidget());	// �����͸� ����Ʈ �����ͷ� ������ �ʿ䰡 �ִ�.
		if (Widget != nullptr)
		{
			Widget->Initialize();
			Widget->BindScreen1();
			Widget->BindHP(m_pStatComp);
			Widget->BindSkill(m_pSkillComp);
		}

		EnableInput(PC);
	}
}

void AMyTest_TopDownCharacter::PostInitializeComponents()
{
	// ������Ʈ ���� �ʱ�ȭ�� ���� 
	Super::PostInitializeComponents();

	// �ִ� �ν��Ͻ� �ҷ����� 
	m_pAnimInstance = CastChecked<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	m_pAnimInstance->OnMontageEnded.AddDynamic(this, &AMyTest_TopDownCharacter::OnAttackMontageEnded);
	m_pAnimInstance->m_OnAttackHit.AddUObject(this, &AMyTest_TopDownCharacter::AttackCheck);
	m_pAnimInstance->m_OnAttackEnd.AddUObject(this, &AMyTest_TopDownCharacter::AttackEnd);
	m_pAnimInstance->m_OnDeathPoint.AddUObject(this, &AMyTest_TopDownCharacter::OnDeathMontageEnded);
	m_pAnimInstance->m_OnHitEnd.AddUObject(this, &AMyTest_TopDownCharacter::OnHitMontageEnded);

	// ���ݿ��� OnDeathCheck�� Broadcast �ȴٸ� Death�Լ� ȣ�� 
	m_pStatComp->OnDeathCheck.AddUObject(this, &AMyTest_TopDownCharacter::Death);

	// Combat Stat			// StatComp�� Init ȣ�� ������ �÷��̾�ĳ������ �����ڶ� PostInitalizeComponents ���̶� 
	//						// ���� ��ġ�� �ξ���Ѵ�. 
	m_AttackRange = m_pStatComp->GetTotalStat().AttackRange;
	m_AttackRadius = m_pStatComp->GetTotalStat().AttackRadius;
	m_DefenseRadius = 250.f;
	m_DetectionRadius = 800.f;
	m_VisibleRadius = 1200.f;

	

	//DEBUG 
	//GetWorld()->GetTimerManager().SetTimer(m_hDebug, this, &AMyTest_TopDownCharacter::Debug, 0.1f, true);


}

void AMyTest_TopDownCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}

void AMyTest_TopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* ���� �Է� �ý��� */
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyTest_TopDownCharacter::Sprint);
	//PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyTest_TopDownCharacter::StopSprint);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyTest_TopDownCharacter::Jump);
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMyTest_TopDownCharacter::StopJumping);
	//PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyTest_TopDownCharacter::Attack);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AMyTest_TopDownCharacter::Look_UpDown);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMyTest_TopDownCharacter::Look_LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Wheel"), this, &AMyTest_TopDownCharacter::Wheel_UpDown);
	PlayerInputComponent->BindAxis(TEXT("Vertical_Keyboard"), this, &AMyTest_TopDownCharacter::MoveToFoward);
	PlayerInputComponent->BindAxis(TEXT("Horizontal_Keyboard"), this, &AMyTest_TopDownCharacter::MoveToRight);


}

void AMyTest_TopDownCharacter::ChangeCameraView()
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

void AMyTest_TopDownCharacter::Attack()
{
	if (m_CharacterState != EBehaviorState::Idle
	&&	m_CharacterState != EBehaviorState::Battle
		)
		return;

	m_pAnimInstance->PlayAttackMontage();
	m_pAnimInstance->JumpToSection_Attack(m_AttackIndex);
	m_AttackIndex = (m_AttackIndex + 1) % 2;	// Attack ����� 2���̹Ƿ� 


	m_CharacterState = EBehaviorState::Attacking;
	m_NextCharacterState = EBehaviorState::Battle;
}

void AMyTest_TopDownCharacter::AttackCheck()
{
	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, this);


	FVector Vec = GetActorForwardVector() * m_AttackRange;
	FVector Center = GetActorLocation() + Vec * 1.0f;
	float HalfHeight = m_AttackRange * 0.5f + m_AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorRightVector()).ToQuat();


	bool bResult = GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		GetActorLocation() + Vec,
		Rotation,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(m_AttackRadius, HalfHeight),
		Params);


	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, m_AttackRadius,
		Rotation, DrawColor, false, 2.f);


	// �ߺ� ���Ÿ� ���ؼ� �ѹ� �ɷ��ش�.
	TArray<AActor*> UniqueResult;
	for (FOverlapResult Src : OverlapResult)
	{
		UniqueResult.AddUnique(Src.GetActor());
	}

	if (bResult)
	{
		for (AActor* Result : UniqueResult)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *Result->GetName());

			FDamageEvent DamageEvent;
			Result->TakeDamage(m_pStatComp->GetAttack(), DamageEvent, GetController(), this);

		}
	}
}

void AMyTest_TopDownCharacter::AttackEnd()
{
	// �Ź� Attack Montage�� Section�� ������ Notify�� ���ؼ� ȣ�� 
	if (m_CharacterState == EBehaviorState::Attacking)
	{

		m_CharacterState = m_NextCharacterState;
	}
}

void AMyTest_TopDownCharacter::OnHit()
{
	//if (m_CharacterState == EBehaviorState::Hit)
	//	return;
	m_CharacterState = EBehaviorState::Hit;
	m_pAnimInstance->PlayHitMontage();
	m_pAnimInstance->JumpToSection_Hit(TEXT("Hit"));

	// Effect
	Effect_Flick(FLinearColor::Red);

	// �ӽ�
	m_pSkillComp->Cancel();
}

void AMyTest_TopDownCharacter::Death()
{
	if (m_CharacterState != EBehaviorState::Die)
	{
		m_CharacterState = EBehaviorState::Die;
		m_pAnimInstance->PlayDeathMontage();
	}
}

void AMyTest_TopDownCharacter::Look_UpDown(float Value)
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

void AMyTest_TopDownCharacter::Look_LeftRight(float Value)
{
	//if (fabs(Value) > 0.00001f)	// ���������� ���ϱ�. ( float�� ���� ) 
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

}

void AMyTest_TopDownCharacter::Wheel_UpDown(float Value)
{
	if (Value == 0.f)return;

	if (m_bShoulderView == true)
	{

	}
	else
	{
		m_CameraBoom->TargetArmLength += (Value * 10.f);
	}

}

void AMyTest_TopDownCharacter::MoveToFoward(float Value)
{
	if (m_bShoulderView == true)
	{
		m_Vertical = Value;
		//AddMovementInput(GetActorForwardVector(), Value);
		GetCharacterMovement()->AddForce(GetActorForwardVector() * Value);
	}
}

void AMyTest_TopDownCharacter::MoveToRight(float Value)
{
	if (m_bShoulderView == true)
	{
		m_Horizontal = -Value; 
		//AddMovementInput(GetActorRightVector(), -Value);
		GetCharacterMovement()->AddForce(GetActorRightVector() * Value);
	}
}

void AMyTest_TopDownCharacter::Click_F()
{
	//CheckForInteractable();

	if (m_CurrentInteractable != nullptr)
	{
	// ������ ä���.
		AMyItem* Item = Cast<AMyItem>(m_CurrentInteractable);
		if (Item != nullptr)
		{
			int32 Index = m_pInventoryComp->FindAddIndex(Item->GetID());
			
			if (Index == -1)// ����  
				return;

			m_pInventoryComp->TakeItem(Item, Index);
		}

		m_CurrentInteractable->Interact_Implementation();

		// ����� ��ȣ�ۿ� ��ü�� ������¿��� �����ֱ� [ �������� ��� ������ ���� ��ȣ�ۿ��� �������� ]
		m_CurrentInteractable = nullptr;
	}

}

void AMyTest_TopDownCharacter::CheckForInteractable()
{
	// Sphere Collision ���� �浹�ؼ� �к��ϱ� 

	//FVector StartTrace = TopDownCameraComponent->GetComponentLocation();
	//FVector EndTrace = (TopDownCameraComponent->GetForwardVector() * m_Reach) + StartTrace;

	FHitResult HitResult;

	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(120.f);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);
	// 
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation(),GetActorRotation().Quaternion(), ECC_WorldDynamic, ExplosionSphere, CQP);


	// ECC_GameTraceChannel3 �����۹ڽ� �浹 ä���̴�.
	// ECC_WorldDynamic ���̳���...
	// ���� ����   Interactable ��ü�� ���� ����ؼ� ����ؾ������� ������ ��� MyItem�� ��ü�� 

	AMyInteractable* InteractableActor = Cast<AMyInteractable>(HitResult.GetActor());
	
	if (InteractableActor == nullptr)
	{
		m_HelpText = TEXT("");
		m_CurrentInteractable = nullptr;
	}
	else
	{
		m_CurrentInteractable = InteractableActor;
		m_HelpText = InteractableActor->GetHelpText();
	}


	// Debug

	//if (bHit)
	//{
	//	DrawDebugSphere(GetWorld(), GetActorLocation(), 120.f, 16.f, FColor::Green, false, 0.1f, 0, 1.0f);
	//}
	//else
	//{
	//	DrawDebugSphere(GetWorld(), GetActorLocation(), 120.f, 16.f, FColor::Red, false, 0.1f, 0, 1.0f);
	//}



}
/// <summary>
/// ĳ���� �ܿ��� UseItem�� �Ѵ�.
/// </summary>
/// <param name="Index">������ ����Ʈ������ ��ȣ </param>
void AMyTest_TopDownCharacter::UseItem(int32 Index)
{
	if (m_pInventoryComp == nullptr)
		return;

	// �ϴ� ������ int32�� �迭�� �� �ޱ� �ϴµ� �̰� �´���... 
	TArray<int32> ItemEffect = m_pInventoryComp->UseItem(Index);
	if (ItemEffect.IsEmpty() == false)
	{
		switch (ItemEffect[0])
		{
		case static_cast<int32>(EItemEffectType::PlusHP):
		{
			m_pStatComp->AddHP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusMP):
		{
			m_pStatComp->AddMP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusSP):
		{
			m_pStatComp->AddSP(ItemEffect[2]);
			break;
		}
		case static_cast<int32>(EItemEffectType::PlusAttack):
		{
			FBaseStatusData Stat{};
			Stat.Attack = ItemEffect[2];
			m_pStatComp->AddBaseStat(Stat);

			break;
		}
		case static_cast<int32>(EItemEffectType::PlusDefence):
		{	
			FBaseStatusData Stat{};
			Stat.Defence = ItemEffect[2];
			m_pStatComp->AddBaseStat(Stat);

			break;
		}

		default:
			break;
		}

	}

	
}
/// <summary>
/// ĳ���� �ܿ��� DropItem�� �Ѵ�.
/// </summary>
/// <param name="Index"></param>
/// <param name="Pos"></param>
void AMyTest_TopDownCharacter::DropItem(int32 Index,FVector Pos)
{
	if (m_pInventoryComp == nullptr)
		return;
	m_pInventoryComp->DropItem(Index, Pos);


}

void AMyTest_TopDownCharacter::HUDUpdate(uint8 Type)
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
				Widget->BindHP(m_pStatComp);
				Widget->UpdateHP();
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
		default :
		{
			break;
		}
	}
	
}

void AMyTest_TopDownCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bINterrupted)
{
	// Attack Montage�� ���� �����ϸ� ȣ�� 
	// TO DO 

}

void AMyTest_TopDownCharacter::OnDeathMontageEnded()
{
	// To Do   ���⿡�� �״� �ִϸ��̼��� ������ ���� ��Ȳ�� ó���� ����. 

	switch (m_CharacterState)
	{
		case EBehaviorState::Die :
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
		default :
		{break; }
	}
	

}

void AMyTest_TopDownCharacter::OnHitMontageEnded()
{
	if (m_CharacterState == EBehaviorState::Hit)
	{
		m_CharacterState = EBehaviorState::Idle;
	}
}

bool AMyTest_TopDownCharacter::HasCurrentInteractable()
{
	if (m_CurrentInteractable != nullptr)
		return true;
	else
		return false;
}

void AMyTest_TopDownCharacter::SetCurrentInteractable(AMyInteractable* Interactable)
{
	 m_CurrentInteractable = Interactable; 
}

void AMyTest_TopDownCharacter::Sprint()
{
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;			// ĳ���� �̵��ӵ�	(�ӽ�) ����� �ٲ���Ѵ�.
		m_CameraBoom->CameraLagSpeed = 8.0f;					// ī�޶� �����ӵ�

	}
}

void AMyTest_TopDownCharacter::SetSprint(float WalkSpeed, float CameraSpeed)
{
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// ĳ���� �̵��ӵ�
		m_CameraBoom->CameraLagSpeed = CameraSpeed;				// ī�޶� �����ӵ�

	}
}

void AMyTest_TopDownCharacter::StopSprint()
{
	// ACharacterBase�� ����� ����. 
	Super::StopSprint();
}

float AMyTest_TopDownCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_pStatComp->GetHP() == 0)
		return 0.f;

	OnHit();

	m_pStatComp->OnAttacked(DamageAmount);
	return DamageAmount;
}

void AMyTest_TopDownCharacter::Debug()
{
	// Debug Off    goto BeginPlay() 
	// ���� ��� �׽�Ʈ  
	URandomSystem* RandomObject = NewObject<URandomSystem>(GetWorld());
	RandomObject->AddList
	(
		TMap<int32, int32>{ {0,20}, { 1,10 }, { 2,30 }, { 3,15 }, { 4,12 }, { 5,13 }}
	);
	FString str = FString::Printf(TEXT("%d"), RandomObject->PickRandom(100));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White,str);
}