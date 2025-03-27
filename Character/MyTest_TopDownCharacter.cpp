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

	// 충돌체크 함수를 일정하게 호출할 타이머 
	//GetWorldTimerManager().SetTimer(m_Timer_ItemCollision,this, &AMyTest_TopDownCharacter::CheckForInteractable, 1.f, true, 1.f);

	AMyTest_TopDownPlayerController* PC = CastChecked<AMyTest_TopDownPlayerController>(GetController());
	if (nullptr != PC)
	{
		// HUDUpdate	바인딩
		PC->OnHUDUpdate.AddUObject(this, &AMyTest_TopDownCharacter::HUDUpdate);
		// HUD Init		초기화
		auto Widget = Cast<UMyHUD>(PC->GetCurrentWidget());	// 포인터를 스마트 포인터로 관리할 필요가 있다.
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
	// 컴포넌트 들이 초기화된 이후 
	Super::PostInitializeComponents();

	// 애님 인스턴스 불러오기 
	m_pAnimInstance = CastChecked<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	m_pAnimInstance->OnMontageEnded.AddDynamic(this, &AMyTest_TopDownCharacter::OnAttackMontageEnded);
	m_pAnimInstance->m_OnAttackHit.AddUObject(this, &AMyTest_TopDownCharacter::AttackCheck);
	m_pAnimInstance->m_OnAttackEnd.AddUObject(this, &AMyTest_TopDownCharacter::AttackEnd);
	m_pAnimInstance->m_OnDeathPoint.AddUObject(this, &AMyTest_TopDownCharacter::OnDeathMontageEnded);
	m_pAnimInstance->m_OnHitEnd.AddUObject(this, &AMyTest_TopDownCharacter::OnHitMontageEnded);

	// 스텟에서 OnDeathCheck가 Broadcast 된다면 Death함수 호출 
	m_pStatComp->OnDeathCheck.AddUObject(this, &AMyTest_TopDownCharacter::Death);

	// Combat Stat			// StatComp의 Init 호출 시점이 플레이어캐릭터의 생성자랑 PostInitalizeComponents 사이라서 
	//						// 현재 위치에 두어야한다. 
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

	/* 기존 입력 시스템 */
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
	m_AttackIndex = (m_AttackIndex + 1) % 2;	// Attack 모션이 2개이므로 


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


	// 중복 제거를 위해서 한번 걸러준다.
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
	// 매번 Attack Montage의 Section이 끝나면 Notify에 의해서 호출 
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

	// 임시
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
	if (fabs(Value) > 0.00001f)	// 오차범위로 비교하기. ( float을 비교중 ) 
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
	//if (fabs(Value) > 0.00001f)	// 오차범위로 비교하기. ( float을 비교중 ) 
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
	// 아이템 채우기.
		AMyItem* Item = Cast<AMyItem>(m_CurrentInteractable);
		if (Item != nullptr)
		{
			int32 Index = m_pInventoryComp->FindAddIndex(Item->GetID());
			
			if (Index == -1)// 실패  
				return;

			m_pInventoryComp->TakeItem(Item, Index);
		}

		m_CurrentInteractable->Interact_Implementation();

		// 사용한 상호작용 객체는 현재상태에서 지워주기 [ 아이템의 경우 쓰고나면 현재 상호작용을 할이유가 ]
		m_CurrentInteractable = nullptr;
	}

}

void AMyTest_TopDownCharacter::CheckForInteractable()
{
	// Sphere Collision 으로 충돌해서 분별하기 

	//FVector StartTrace = TopDownCameraComponent->GetComponentLocation();
	//FVector EndTrace = (TopDownCameraComponent->GetForwardVector() * m_Reach) + StartTrace;

	FHitResult HitResult;

	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(120.f);
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);
	// 
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation(),GetActorRotation().Quaternion(), ECC_WorldDynamic, ExplosionSphere, CQP);


	// ECC_GameTraceChannel3 아이템박스 충돌 채널이다.
	// ECC_WorldDynamic 다이나믹...
	// 비교할 액터   Interactable 객체를 만들어서 상속해서 사용해야하지만 지금은 없어서 MyItem이 대체중 

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
/// 캐릭터 단에서 UseItem을 한다.
/// </summary>
/// <param name="Index">아이템 리스트에서의 번호 </param>
void AMyTest_TopDownCharacter::UseItem(int32 Index)
{
	if (m_pInventoryComp == nullptr)
		return;

	// 일단 값들이 int32라서 배열로 쭉 받긴 하는데 이게 맞는지... 
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
/// 캐릭터 단에서 DropItem을 한다.
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
	// Attack Montage가 끝에 도달하면 호출 
	// TO DO 

}

void AMyTest_TopDownCharacter::OnDeathMontageEnded()
{
	// To Do   여기에서 죽는 애니메이션이 끝나고 나서 상황을 처리해 주자. 

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


			// 임시용 게임 종료 
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
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;			// 캐릭터 이동속도	(임시) 상수는 바꿔야한다.
		m_CameraBoom->CameraLagSpeed = 8.0f;					// 카메라 지연속도

	}
}

void AMyTest_TopDownCharacter::SetSprint(float WalkSpeed, float CameraSpeed)
{
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// 캐릭터 이동속도
		m_CameraBoom->CameraLagSpeed = CameraSpeed;				// 카메라 지연속도

	}
}

void AMyTest_TopDownCharacter::StopSprint()
{
	// ACharacterBase랑 기능이 같다. 
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
	// 랜덤 기능 테스트  
	URandomSystem* RandomObject = NewObject<URandomSystem>(GetWorld());
	RandomObject->AddList
	(
		TMap<int32, int32>{ {0,20}, { 1,10 }, { 2,30 }, { 3,15 }, { 4,12 }, { 5,13 }}
	);
	FString str = FString::Printf(TEXT("%d"), RandomObject->PickRandom(100));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White,str);
}