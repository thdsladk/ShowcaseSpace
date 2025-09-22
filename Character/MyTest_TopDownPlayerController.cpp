// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest_TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"

#include "MyTest_TopDownGameMode.h"
#include "Character/CharacterBase.h"
#include "AnimInstanceBase.h"
#include "Interface/PlayerControllInterface.h"
#include "AbilityComponent.h"

#include "Interface/HighlightInterface.h"

#include "MyHUD.h"
#include "MyInventoryWidget.h"
#include "StatWidget.h"

// (임시) 스킬을 컨트롤러가 아는것이 맞는가 ?
#include "Header/SkillEnum.h"


AMyTest_TopDownPlayerController::AMyTest_TopDownPlayerController()
	:m_bGamePlayControllable(true) , m_bSystemControllable(true)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	m_CachedDestination = FVector::ZeroVector;
	m_FollowTime = 0.f;

	// 마우스 오버 이벤트 ON
	bEnableMouseOverEvents = true;

	// 코드에서 에셋을 찾아서 호출하기 보다는 에디터에서 블루프린트로 접근해서 세팅해주자 [ 그게 더 안전 ]
	SetDestKeyAction.Add(EKeys::Q, nullptr);
	SetDestKeyAction.Add(EKeys::W, nullptr);
	SetDestKeyAction.Add(EKeys::E, nullptr);
	SetDestKeyAction.Add(EKeys::R, nullptr);
	SetDestKeyAction.Add(EKeys::V, nullptr);
	SetDestKeyAction.Add(EKeys::P, nullptr);
	SetDestKeyAction.Add(EKeys::F, nullptr);
	SetDestKeyAction.Add(EKeys::A, nullptr);
	SetDestKeyAction.Add(EKeys::S, nullptr);
	SetDestKeyAction.Add(EKeys::D, nullptr);
	SetDestKeyAction.Add(EKeys::G, nullptr);
	SetDestKeyAction.Add(EKeys::H, nullptr);
	SetDestKeyAction.Add(EKeys::J, nullptr);
	SetDestKeyAction.Add(EKeys::T, nullptr);
	
	SetDestKeyAction.Add(EKeys::MouseWheelAxis, nullptr);
	SetDestKeyAction.Add(EKeys::Tab, nullptr);
	SetDestKeyAction.Add(EKeys::SpaceBar, nullptr);
	SetDestKeyAction.Add(EKeys::LeftShift, nullptr);
	
	// HUD										
	static ConstructorHelpers::FClassFinder<UMyHUD> UI_HUD(TEXT("/Game/TopDown/Blueprints/UI/BP_MyHUD.BP_MyHUD_C"));
	if (UI_HUD.Succeeded())
	{
		HUD_Class = UI_HUD.Class;

		// Create the widget instance.
		m_CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (m_CurrentWidget)
		{
			// Add the widget to the viewport so that it becomes visible on the screen.
			m_CurrentWidget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail HUD"));
	}

	static ConstructorHelpers::FClassFinder<UMyInventoryWidget> Inventory_HUD(TEXT("/Game/TopDown/Blueprints/UI/Inventory/WBP_MyInventoryWidget.WBP_MyInventoryWidget_C"));
	if (Inventory_HUD.Succeeded())
	{
		m_InventoryWidget = Inventory_HUD.Class;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail Inventory HUD"));
	}

	static ConstructorHelpers::FClassFinder<UStatWidget> Status_HUD(TEXT("/Game/TopDown/Blueprints/UI/BP_StatHUD.BP_StatHUD_C"));
	if (Status_HUD.Succeeded())
	{
		m_StatusWidget = Status_HUD.Class;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail Status HUD"));
	}

}

void AMyTest_TopDownPlayerController::ApplyHUDChange()
{
	if (m_CurrentWidget != nullptr)
	{
		m_CurrentWidget->RemoveFromParent();
	}

	switch (m_HUDState)
	{
	case EHUDState::EIngame:
	{
		ApplyHUD(HUD_Class, true, true);
		break;
	}
	case EHUDState::EInventory:
	{
		ApplyHUD(m_InventoryWidget, true, true);
		break;
	}
	case EHUDState::EShop:
	{
		ApplyHUD(m_ShopWidget, true, true);
		break;
	}
	case EHUDState::EStatus:
	{
		ApplyHUD(m_StatusWidget, true, true);
		break;
	}
	case EHUDState::ESkill:
	{
		ApplyHUD(HUD_Class, true, true);
		break;
	}
	default:
		break;
	}
}

void AMyTest_TopDownPlayerController::ChangeHUDState(uint8 State)
{
	m_HUDState = State;
	ApplyHUDChange();
}

bool AMyTest_TopDownPlayerController::ApplyHUD(TSubclassOf<UUserWidget> Widget, bool bShowMouse, bool EnableClickEvent)
{
	bShowMouseCursor = bShowMouse;
	bEnableClickEvents = EnableClickEvent;

	m_CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);

	if (m_CurrentWidget != nullptr)
	{
		m_CurrentWidget->AddToViewport();
		// Widget이 변경 될때마다 알리자 . 
		OnHUDUpdate.Broadcast(m_HUDState);

		return true;
	}

	return false;
}

void AMyTest_TopDownPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// 빙의가 안되어서 여기서 해보자 . 
	// 
	// 공격이 끝났을때 호출되도록.
	APawn* Self = GetPawn();
	ACharacterBase* CharacterBase = CastChecked<ACharacterBase>(Self);
	CastChecked<ACharacterBase>(GetPawn())->OnAttackEnd.AddUObject(this, &AMyTest_TopDownPlayerController::OnGamePlayControlReturned);
	// (임시) AnimInstanceBase를 컨트롤러에서 꺼내서 바인딩해도 되는가 ?
	CastChecked<UAnimInstanceBase>(CharacterBase->GetMesh()->GetAnimInstance())->m_OnSprintEnd.AddUObject(this, &AMyTest_TopDownPlayerController::OnGamePlayControlReturned);

	if (m_HUDState == EHUDState::EIngame)
	{
		if (m_CurrentWidget != nullptr)
		{
			CastChecked<UMyHUD>(m_CurrentWidget)->m_OnSkill.AddUObject(this, &AMyTest_TopDownPlayerController::Click_Skill);
		}
	}
}

void AMyTest_TopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EIComp = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events (Left)
		EIComp->BindAction(SetDestinationLClickAction, ETriggerEvent::Started, this, &AMyTest_TopDownPlayerController::OnInputStarted);
		EIComp->BindAction(SetDestinationLClickAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::OnSetDestinationTriggered);
		EIComp->BindAction(SetDestinationLClickAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::OnSetDestinationReleased);
		EIComp->BindAction(SetDestinationLClickAction, ETriggerEvent::Canceled, this, &AMyTest_TopDownPlayerController::OnSetDestinationReleased);

		// Setup mouse input events (Right)
		EIComp->BindAction(SetDestinationRClickAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::ClickRMouse);

		// Setup touch input events
		EIComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMyTest_TopDownPlayerController::OnInputStarted);
		EIComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::OnTouchTriggered);
		EIComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::OnTouchReleased);
		EIComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMyTest_TopDownPlayerController::OnTouchReleased);

		// Setup Key events
		EIComp->BindAction(SetDestKeyAction[EKeys::Q], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Q);
		EIComp->BindAction(SetDestKeyAction[EKeys::W], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_W);
		EIComp->BindAction(SetDestKeyAction[EKeys::E], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_E);
		EIComp->BindAction(SetDestKeyAction[EKeys::R], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_R);
		EIComp->BindAction(SetDestKeyAction[EKeys::V], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_V);
		EIComp->BindAction(SetDestKeyAction[EKeys::P], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Click_P);
		EIComp->BindAction(SetDestKeyAction[EKeys::F], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_F);
		EIComp->BindAction(SetDestKeyAction[EKeys::A], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Click_A);
		EIComp->BindAction(SetDestKeyAction[EKeys::S], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_S);
		EIComp->BindAction(SetDestKeyAction[EKeys::D], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_D);
		EIComp->BindAction(SetDestKeyAction[EKeys::D], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_D);

		EIComp->BindAction(SetDestKeyAction[EKeys::G], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_G);
		EIComp->BindAction(SetDestKeyAction[EKeys::H], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_H);
		EIComp->BindAction(SetDestKeyAction[EKeys::J], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_J);
		EIComp->BindAction(SetDestKeyAction[EKeys::T], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_T);

		EIComp->BindAction(SetDestKeyAction[EKeys::Tab], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Click_Tab);
		EIComp->BindAction(SetDestKeyAction[EKeys::SpaceBar], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Space);
		EIComp->BindAction(SetDestKeyAction[EKeys::SpaceBar], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_Space);
		EIComp->BindAction(SetDestKeyAction[EKeys::LeftShift], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Shift);
		EIComp->BindAction(SetDestKeyAction[EKeys::LeftShift], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_Shift);
		
		// Setup MoveKet evnents
		EIComp->BindAction(MoveKeyAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Move);

	}

	//Inventory_Notify.AddUObject(this, &AMyTest_TopDownPlayerController::Click_Tab);
	CastChecked<UMyHUD>(m_CurrentWidget)->m_OnInven.AddUObject(this, &AMyTest_TopDownPlayerController::Click_Tab);


}

#pragma region Input Action
void AMyTest_TopDownPlayerController::OnInputStarted()
{
	if (m_bGamePlayControllable == false)return;
	StopMovement();
}

// Triggered every frame when the input is held down
void AMyTest_TopDownPlayerController::OnSetDestinationTriggered()
{
	if (m_bGamePlayControllable == false)return;

	// We flag that the input is being pressed
	m_FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (m_bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		// 커서일 경우
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful == true)
	{
		m_CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (m_CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMyTest_TopDownPlayerController::OnSetDestinationReleased()
{
	if (m_bGamePlayControllable == false)return;
	// If it was a short press
	if (m_FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, m_CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, m_CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

		// 상호작용 확인선
		if (PerformLineTrace() == true)
		{
			// 상호작용하는 액터를 눌러도 이동도 가능 

		}
	}

	m_FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AMyTest_TopDownPlayerController::OnTouchTriggered()
{
	if (m_bGamePlayControllable == false)return;

	m_bIsTouch = true;
	OnSetDestinationTriggered();
}

void AMyTest_TopDownPlayerController::OnTouchReleased()
{
	if (m_bGamePlayControllable == false)return;

	m_bIsTouch = false;
	OnSetDestinationReleased();
}

void AMyTest_TopDownPlayerController::ClickRMouse()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickRMouse();
}

void AMyTest_TopDownPlayerController::Click_Tab()
{
	if (m_bSystemControllable == false)return;

	if (m_HUDState == AMyTest_TopDownPlayerController::EHUDState::EInventory) // 인벤토리가 열려 있으면 
	{
		ChangeHUDState(AMyTest_TopDownPlayerController::EHUDState::EIngame);
		SetPause(false);
		
	}
	else						  // 인벤토리가 닫혀 있으면
	{
		ChangeHUDState(AMyTest_TopDownPlayerController::EHUDState::EInventory);
		SetPause(true);
		
		// InventoryWidget Update Call 
		UMyInventoryWidget* InventoryWidget = CastChecked<UMyInventoryWidget>(m_CurrentWidget);
		if (InventoryWidget)
		{
			InventoryWidget->UpdateWidget();
		}
	}

}

void AMyTest_TopDownPlayerController::Click_F()
{
	if (m_bGamePlayControllable == false)return;
	//UE_LOG(LogTemp, Log, TEXT(" Interact ON "));

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->Click_F();
}

void AMyTest_TopDownPlayerController::Click_P()
{
	if (m_bSystemControllable == false)return;

	if (m_HUDState == AMyTest_TopDownPlayerController::EHUDState::EStatus) // 상태창이 열려 있으면 
	{
		ChangeHUDState(AMyTest_TopDownPlayerController::EHUDState::EIngame);
		SetPause(false);

	}
	else						  // 상태창이 닫혀 있으면
	{
		ChangeHUDState(AMyTest_TopDownPlayerController::EHUDState::EStatus);
		SetPause(true);

		// InventoryWidget Update Call 
		//UStatWidget* StatWidget = CastChecked<UStatWidget>(GameMode->GetCurrentWidget());
		//if (StatWidget)
		//{
		//	StatWidget->UpdateWidget();
		//}
	}


}

void AMyTest_TopDownPlayerController::Click_Q()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_Q);
}

void AMyTest_TopDownPlayerController::Click_W()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_W);

}

void AMyTest_TopDownPlayerController::Click_E()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_E);
}

void AMyTest_TopDownPlayerController::Click_R()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_R);
}

void AMyTest_TopDownPlayerController::Click_V()
{
	if (m_bSystemControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_V);
}

void AMyTest_TopDownPlayerController::Click_A()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_A);

}

void AMyTest_TopDownPlayerController::Click_S()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_S);
}

void AMyTest_TopDownPlayerController::Click_D()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_D);

	// 방어라서 제어 불가로 만들기
	m_bGamePlayControllable = false;
}

void AMyTest_TopDownPlayerController::Click_G()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_G);
}

void AMyTest_TopDownPlayerController::Click_H()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_H);
}

void AMyTest_TopDownPlayerController::Click_J()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_J);
}

void AMyTest_TopDownPlayerController::Click_T()
{
	if (m_bGamePlayControllable == false)return;
	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_T);

}

void AMyTest_TopDownPlayerController::Click_Space()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_Space);

}

void AMyTest_TopDownPlayerController::Click_Shift()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ClickButton(EKey::Key_Shift);
}

void AMyTest_TopDownPlayerController::Release_Space()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ReleaseButton(EKey::Key_Space);

	// 공격이라서 제어 불가로 만들기
	m_bGamePlayControllable = false;
}

void AMyTest_TopDownPlayerController::Release_Shift()
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->Release_Shift();

	// 달리기 마무리중 제어 불가로 만들기
	m_bGamePlayControllable = false;
}

void AMyTest_TopDownPlayerController::Release_D()
{
	//if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->ReleaseButton(EKey::Key_D);

	// 제어 가능 만들기
	m_bGamePlayControllable = true;

}

void AMyTest_TopDownPlayerController::Move(const FInputActionValue& Value)
{
	if (m_bGamePlayControllable == false)return;

	IPlayerControllInterface* Interface = CastChecked<IPlayerControllInterface>(GetCharacter());
	Interface->Keyboard_Move(Value);
}

void AMyTest_TopDownPlayerController::Click_Skill(uint8 SkillNum)
{
	switch (static_cast<::ESkill>(SkillNum))
	{
		case ESkill::Skill_Q:
		{
			Click_Q();
			break;
		}
		case ESkill::Skill_W:
		{
			Click_T();
			break;
		}
		case ESkill::Skill_E:
		{
			Click_E();
			break;
		}
		case ESkill::Skill_R:
		{
			Click_R();
			break;
		}
		default:
			break;
	}
}
#pragma endregion

#pragma region GameMode Function
void AMyTest_TopDownPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnGameRetryCount(NewScore);
}

void AMyTest_TopDownPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AMyTest_TopDownPlayerController::GameOver()
{
	K2_OnGameOver();
}
#pragma endregion

bool AMyTest_TopDownPlayerController::PerformLineTrace()
{

	// 마우스 위치 가져오기
	float MouseX, MouseY;
	if (GetMousePosition(MouseX, MouseY) == true)
	{
		FVector WorldLocation, WorldDirection;

		// 화면 좌표를 월드 좌표로 변환
		DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

		FVector Start = WorldLocation;
		FVector End = WorldLocation + (WorldDirection * 10000.0f); // 라인 길이 설정 ( 상수 보다는 지면 같은 기준이 있어야 할거같다. ) 

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		//CollisionParams.AddIgnoredActor(GetPawn()); // 플레이어 자신 제외 , 충돌이 적만 하도록 되어있어서 괜찮다.

		// 라인 트레이스 수행   // ECC_Visibility 보이는 것들에 대해서. //ECC_GameTraceChannel2 , Attack에 대한 채널.
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel2, CollisionParams) ==true)
		{
			AActor* TargetActor = HitResult.GetActor();
			if (m_TargetActor == TargetActor)
			{
				// 이미 선택된 액터를 누른 경우.
			}
			else
			{
				IHighlightInterface* HighlightInterface = CastChecked<IHighlightInterface>(TargetActor);
				// EnemyActor를 클릭한 경우 처리
				UE_LOG(LogTemp, Warning, TEXT(" Hit NPC : %s"), *TargetActor->GetName());


				// 하이라이트 ON
				HighlightInterface->HighlightActor();

				// 이전 하이라이트 후 처리
				if (m_TargetActor != nullptr)
				{
					IHighlightInterface* preHighlightInterface = CastChecked<IHighlightInterface>(m_TargetActor);
					preHighlightInterface->UnHighlightActor();

				}

				// 타갯 갱신
				m_TargetActor = TargetActor;
			}


			// Debug
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f);

			return true;
		}
	}

	return false;
}

