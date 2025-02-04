// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest_TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "MyTest_TopDownCharacter.h"
#include "SkillComponent.h"

#include "Components/WidgetComponent.h"
#include "MyInventoryWidget.h"
#include "StatWidget.h"
#include "EnhancedInputComponent.h"
#include "MyTest_TopDownGameMode.h"
#include "MyHUD.h"

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
	AMyTest_TopDownGameMode* GameMode = CastChecked<AMyTest_TopDownGameMode>(GetWorld()->GetAuthGameMode());
	CastChecked<UMyHUD>(GameMode->GetCurrentWidget())->m_OnInven.AddUObject(this, &AMyTest_TopDownPlayerController::Click_Tab);
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
	AMyTest_TopDownGameMode* GameMode = CastChecked<AMyTest_TopDownGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->GetHUDState() == AMyTest_TopDownGameMode::EState::EInventory) // 인벤토리가 열려 있으면 
	{
		GameMode->ChangeHUDState(AMyTest_TopDownGameMode::EState::EIngame);
		SetPause(false);
		
	}
	else						  // 인벤토리가 닫혀 있으면
	{
		GameMode->ChangeHUDState(AMyTest_TopDownGameMode::EState::EInventory);
		SetPause(true);
		
		// InventoryWidget Update Call 
		UMyInventoryWidget* InventoryWidget = CastChecked<UMyInventoryWidget>(GameMode->GetCurrentWidget());
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
	AMyTest_TopDownGameMode* GameMode = CastChecked<AMyTest_TopDownGameMode>(GetWorld()->GetAuthGameMode());
	
	if (GameMode->GetHUDState() == AMyTest_TopDownGameMode::EState::EStatus) // 상태창이 열려 있으면 
	{
		GameMode->ChangeHUDState(AMyTest_TopDownGameMode::EState::EIngame);
		SetPause(false);

	}
	else						  // 상태창이 닫혀 있으면
	{
		GameMode->ChangeHUDState(AMyTest_TopDownGameMode::EState::EStatus);
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
