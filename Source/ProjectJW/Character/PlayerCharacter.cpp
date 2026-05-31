// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Character/CharacterControlData.h"


//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	// Create a camera boom (pulls in towards the player if there is a collision)
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_CameraBoom"));
	m_CameraBoom->SetupAttachment(RootComponent);
	m_CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	m_CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	m_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_FollowCamera"));
	m_FollowCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	m_WhealScale = 10.f;
	m_OriginCameraLagSpeed = m_CameraBoom->CameraLagSpeed;


}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::SetCharacterControlData(const UCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	m_CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	m_CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	m_CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	m_CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	m_CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	m_CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	m_CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

#pragma region Movement Interface 
void APlayerCharacter::ShoulderMove(const FVector2D& Value)
{
	Super::ShoulderMove(Value);
}

void APlayerCharacter::ShoulderLook(const FVector2D& Value)
{
	Super::ShoulderLook(Value);
}

void APlayerCharacter::QuaterMove(FVector2D Value)
{
	Super::QuaterMove(Value);
}

void APlayerCharacter::WhealInput(const float& Value)
{
	//Super::WhealInput(Value);
	m_CameraBoom->TargetArmLength += m_WhealScale * Value;

}

void APlayerCharacter::RotateToInterp(FVector2D Value, float InterpSpeed)
{
	Super::RotateToInterp(Value, InterpSpeed);

}

void APlayerCharacter::StartSprint()
{
	Super::StartSprint();

	ScaleCameraSpeed(2.f);
}

void APlayerCharacter::StopSprint()
{
	Super::StopSprint();

	InitCameraSpeed();
}

void APlayerCharacter::SetSprint(float Ratio)
{
	Super::SetSprint(Ratio);

	ScaleCameraSpeed(Ratio);
}
#pragma endregion

void APlayerCharacter::InitCameraSpeed()
{
	m_CameraBoom->CameraLagSpeed = m_OriginCameraLagSpeed;
}

void APlayerCharacter::SetCameraSpeed(float NewCameraSpeed)
{
	m_CameraBoom->CameraLagSpeed = NewCameraSpeed;
}

void APlayerCharacter::ScaleCameraSpeed(float Ratio)
{
	m_CameraBoom->CameraLagSpeed *= Ratio;
}





