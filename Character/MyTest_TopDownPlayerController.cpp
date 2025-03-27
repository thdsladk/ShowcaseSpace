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
#include "MyTest_TopDownCharacter.h"
#include "SkillComponent.h"


#include "MyHUD.h"
#include "MyInventoryWidget.h"
#include "StatWidget.h"


AMyTest_TopDownPlayerController::AMyTest_TopDownPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	//SetDestinationKeyAction.Init(nullptr, static_cast<int32>(Key_End));

	// 코드에서 에셋을 찾아서 호출하기 보다는 에디터에서 블루프린트로 접근해서 세팅해주자 [ 그게 더 안전 ]
	SetDestinationKeyAction.Add(EKeys::Q, nullptr);
	SetDestinationKeyAction.Add(EKeys::W, nullptr);
	SetDestinationKeyAction.Add(EKeys::E, nullptr);
	SetDestinationKeyAction.Add(EKeys::R, nullptr);
	SetDestinationKeyAction.Add(EKeys::V, nullptr);
	SetDestinationKeyAction.Add(EKeys::P, nullptr);
	SetDestinationKeyAction.Add(EKeys::F, nullptr);
	SetDestinationKeyAction.Add(EKeys::MouseWheelAxis, nullptr);
	SetDestinationKeyAction.Add(EKeys::Tab, nullptr);
	SetDestinationKeyAction.Add(EKeys::SpaceBar, nullptr);
	SetDestinationKeyAction.Add(EKeys::LeftShift, nullptr);
	
	// HUD 
	static ConstructorHelpers::FClassFinder<UMyHUD> UI_HUD(TEXT("/Game/TopDown/UI/BP_MyHUD.BP_MyHUD_C"));
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

	static ConstructorHelpers::FClassFinder<UMyInventoryWidget> Inventory_HUD(TEXT("/Game/TopDown/UI/Inventory/WBP_MyInventoryWidget.WBP_MyInventoryWidget_C"));
	if (Inventory_HUD.Succeeded())
	{
		m_InventoryWidget = Inventory_HUD.Class;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Fail Inventory HUD"));
	}

	static ConstructorHelpers::FClassFinder<UStatWidget> Status_HUD(TEXT("/Game/TopDown/UI/BP_StatHUD.BP_StatHUD_C"));
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
	//AMyTest_TopDownCharacter* Character = Cast<AMyTest_TopDownCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
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

}

void AMyTest_TopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events (Left)
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Started, this, &AMyTest_TopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationLClickAction, ETriggerEvent::Canceled, this, &AMyTest_TopDownPlayerController::OnSetDestinationReleased);

		// Setup mouse input events (Right)
		EnhancedInputComponent->BindAction(SetDestinationRClickAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::ClickRMouse);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMyTest_TopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMyTest_TopDownPlayerController::OnTouchReleased);

		// Setup Key events
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::Q], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Q);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::W], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_W);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::E], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_E);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::R], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_R);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::V], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_V);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::P], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Click_P);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::F], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_F);
																	   
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::Tab], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Click_Tab);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::SpaceBar], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Space);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::SpaceBar], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_Space);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::LeftShift], ETriggerEvent::Triggered, this, &AMyTest_TopDownPlayerController::Click_Shift);
		EnhancedInputComponent->BindAction(SetDestinationKeyAction[EKeys::LeftShift], ETriggerEvent::Completed, this, &AMyTest_TopDownPlayerController::Release_Shift);

	}

	//Inventory_Notify.AddUObject(this, &AMyTest_TopDownPlayerController::Click_Tab);
	CastChecked<UMyHUD>(m_CurrentWidget)->m_OnInven.AddUObject(this, &AMyTest_TopDownPlayerController::Click_Tab);
}

void AMyTest_TopDownPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AMyTest_TopDownPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMyTest_TopDownPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AMyTest_TopDownPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AMyTest_TopDownPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AMyTest_TopDownPlayerController::ClickRMouse()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Attack();
}

void AMyTest_TopDownPlayerController::Click_Tab()
{

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
	//UE_LOG(LogTemp, Log, TEXT(" Interact ON "));

	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Click_F();
}

void AMyTest_TopDownPlayerController::Click_P()
{
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
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent().Click_Q();
	
}

void AMyTest_TopDownPlayerController::Click_W()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent().Click_W();
}

void AMyTest_TopDownPlayerController::Click_E()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent().Click_E();
}

void AMyTest_TopDownPlayerController::Click_R()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->GetSkillComponent().Click_R();
}

void AMyTest_TopDownPlayerController::Click_V()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->ChangeCameraView();
}

void AMyTest_TopDownPlayerController::Click_Space()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Jump();
}

void AMyTest_TopDownPlayerController::Click_Shift()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->Sprint();
}

void AMyTest_TopDownPlayerController::Release_Space()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->StopJumping();
}

void AMyTest_TopDownPlayerController::Release_Shift()
{
	AMyTest_TopDownCharacter* MyCharacter = CastChecked<AMyTest_TopDownCharacter>(GetCharacter());
	MyCharacter->StopSprint();
}

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
