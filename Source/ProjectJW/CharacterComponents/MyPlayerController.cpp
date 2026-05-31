// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Character/CharacterBase.h"
#include "Character/PlayerCharacter.h"
#include "Character/CharacterControlData.h"
//#include "Header/JWHeader.h"
#include "FunctionLibrary/JWFunctionLibrary.h"

AMyPlayerController::AMyPlayerController()
	:m_OffsetYaw(0.0f)
	,m_IsRotateToMouse(false)
{
	m_CurrentCharacterControlType = ECharacterControlType::Quater;
	//m_ClickActions.Add(FName("ChangeControl"), nullptr);
	//m_ClickActions.Add(FName("ToggleMenu"), nullptr);

	m_ControlMode = EPlayerControlMode::NormalMode;
	m_CachedInputVector = FVector2D::ZeroVector;
	m_VisibleMenu = false;
	m_VisibleDeathWidget = false;
}

void AMyPlayerController::BeginPlay()
{
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(m_DefaultMappingContext, 0);
	}


	SetCharacterControl(m_CurrentCharacterControlType);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) 
	{

		EnhancedInputComponent->BindAction(m_ClickActions_InputID[EInputID::ChangeControl], ETriggerEvent::Triggered, this, &AMyPlayerController::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(m_ClickActions_InputID[EInputID::ToggleMenu], ETriggerEvent::Triggered, this, &AMyPlayerController::ToggleMenu);

		EnhancedInputComponent->BindAction(m_ShoulderMoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::ShoulderMove);
		EnhancedInputComponent->BindAction(m_ShoulderLookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::ShoulderLook);
		EnhancedInputComponent->BindAction(m_ShoulderMoveAction, ETriggerEvent::Completed, this, &AMyPlayerController::ReleaseMove);
		EnhancedInputComponent->BindAction(m_QuaterMoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::QuaterMove);
		EnhancedInputComponent->BindAction(m_QuaterMoveAction, ETriggerEvent::Completed, this, &AMyPlayerController::ReleaseMove);
		EnhancedInputComponent->BindAction(m_QuaterMouseAction, ETriggerEvent::Triggered, this, &AMyPlayerController::QuaterMouse);
	
		EnhancedInputComponent->BindAction(m_WhealAction, ETriggerEvent::Triggered, this, &AMyPlayerController::WhealInput);


	}

}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_IsRotateToMouse == true)
	{
		RotateCharacterToMouse();
	}
}

void AMyPlayerController::UpdateCharacterControl() 
{
	SetCharacterControl(m_CurrentCharacterControlType);
}

void AMyPlayerController::ChangeCharacterControl()
{
	m_CurrentCharacterControlType = ECharacterControlType((uint8(m_CurrentCharacterControlType) + uint8(ECombatMode::End)) % uint8(ECharacterControlType::End));

	// 인자로 값 변경.
	SetCharacterControl(m_CurrentCharacterControlType);
}

void AMyPlayerController::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UCharacterControlData* NewCharacterControl = m_CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	CastChecked<ACharacterBase>(GetPawn())->SetCharacterControlData(NewCharacterControl);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	m_CurrentCharacterControlType = NewCharacterControlType;
}

void AMyPlayerController::ShoulderMove(const FInputActionValue& Value)
{
	if (m_ControlMode == EPlayerControlMode::NormalMode ||
		m_ControlMode == EPlayerControlMode::NoSystemInput)
	{
		SetCachedInputVector(Value.Get<FVector2D>());
		CastChecked<APlayerCharacter>(GetPawn())->ShoulderMove(Value.Get<FVector2D>());
	}
}

void AMyPlayerController::ShoulderLook(const FInputActionValue& Value)
{
	CastChecked<APlayerCharacter>(GetPawn())->ShoulderLook(Value.Get<FVector2D>());
}

void AMyPlayerController::QuaterMove(const FInputActionValue& Value)
{
	if (m_ControlMode == EPlayerControlMode::NormalMode ||
		m_ControlMode == EPlayerControlMode::NoSystemInput)
	{
		SetCachedInputVector(Value.Get<FVector2D>());
		CastChecked<APlayerCharacter>(GetPawn())->QuaterMove(Value.Get<FVector2D>());
	}
}

void AMyPlayerController::QuaterMouse(const FInputActionValue& Value)
{
	// 이것은 클릭(좌우 모두 가능)이 눌리고 마우스 축이 이동할때 값이 들어온다.
	SetMouseMove(Value.Get<FVector2D>());
	if (m_CachedMouseMove.IsNearlyZero() == false)
	{
		// 여기서 마우스 이동에 대한 Broadcast를 해주자.
		if (ChangeMouseMove.IsBound()==true)
		{
			ChangeMouseMove.Broadcast();
		}
	}
}

void AMyPlayerController::ReleaseMove(const FInputActionValue& Value)
{
	if (m_ControlMode == EPlayerControlMode::NormalMode ||
		m_ControlMode == EPlayerControlMode::NoSystemInput)
	{
		CastChecked<APlayerCharacter>(GetPawn())->ReleaseMove(Value.Get<FVector2D>());
	}
}

void AMyPlayerController::WhealInput(const FInputActionValue& Value)
{
	CastChecked<APlayerCharacter>(GetPawn())->WhealInput(Value.Get<float>());
}

void AMyPlayerController::SetCombatMode(uint8 Mode)
{
	m_CombatMode = ECombatMode(Mode); 
	ChangeCombatMode.Broadcast();

	// Update CurrentCharacterControlType			// 계속 두개의 열거형을 맞춰줘야해서 코드가 비대해지는 문제가 있다...
	uint8 Index = uint8(m_CurrentCharacterControlType) / uint8(ECombatMode::End);
	m_CurrentCharacterControlType = ECharacterControlType(Index * uint8(ECombatMode::End) + uint8(m_CombatMode));
	SetCharacterControl(m_CurrentCharacterControlType);
}

const FVector2D& AMyPlayerController::GetCachedInputVector()
{
	return m_CachedInputVector;
}

void AMyPlayerController::SetCachedInputVector(FVector2D Value)
{
	m_CachedInputVector = Value;
}

void AMyPlayerController::RotateCharacterToMouse()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, true, HitResult) == true)
	{
		const FVector& ImpactPoint = HitResult.ImpactPoint;
		ACharacterBase* MyCharacter = Cast<ACharacterBase>(GetPawn());
		if (MyCharacter != nullptr)
		{
			FVector CharLoc = MyCharacter->GetActorLocation();
			FVector TargetDir = (ImpactPoint - CharLoc).GetSafeNormal();
			TargetDir.Z = 0.f; // Z축 고정

			FRotator LookAtRot = TargetDir.Rotation();
			if (m_CombatMode == ECombatMode::RangeMode)
			{
				// (임시) 애니메이션으로 인해서 회전을 조금 줘야 활모션이 맞아떨어지는 문제가있다. 
				m_OffsetYaw = FMath::Clamp(m_OffsetYaw, 0.f, 360.f);
				LookAtRot.Yaw += m_OffsetYaw;//67.5f;// 45.f;
			}
			LookAtRot.Pitch = 0.f;
			LookAtRot.Roll = 0.f;

			//MyCharacter->RotateToInterp(FVector2D(TargetDir.X, TargetDir.Y));
			MyCharacter->SetActorRotation(LookAtRot);

			// Debug
			if (UJWFunctionLibrary::IsDebugMode(GetPawn()) == true)
			{
				UJWFunctionLibrary::Debug_Direction(GetPawn(), TargetDir, 200.f, FColor::Blue, 0.1f);
				UJWFunctionLibrary::Debug_Direction(GetPawn(), MyCharacter->GetActorForwardVector(), 200.f, FColor::Red, 0.1f);
				UJWFunctionLibrary::Debug_ShapeSingleByChannel(GetPawn(), ImpactPoint, ImpactPoint, FCollisionShape::MakeSphere(10.f), true);
			}
		}


	}

}

FGameplayTag AMyPlayerController::GetCombatModeTag()
{
	return FGameplayTag();
}
