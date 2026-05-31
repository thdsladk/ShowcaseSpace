// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "CharacterComponents/JWMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstanceBase.h"
#include "Character/CharacterControlData.h"
#include "Data/DA_CollisionTeam.h"
#include "Physics/JWCollision.h"
#include "MotionWarpingComponent.h"
#include "NiagaraComponent.h"

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UJWMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	m_MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MontionWarpingComponent"));
	m_GhostTrailNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	m_FootStepNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FootStepNiagaraComponent"));
	
	m_MotionWarpingComponent->ComponentTags.Add(TEXT("MontionWarpingComponent"));
	m_GhostTrailNiagaraSystem->ComponentTags.Add(TEXT("NiagaraComponent"));
	m_FootStepNiagaraSystem->ComponentTags.Add(TEXT("FootStepNiagaraComponent"));

	// Init Flags
	m_HasRootMotion = false;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_HasRootMotion == false && m_MotionWarpingComponent != nullptr)
	{
		m_MotionWarpingComponent->DestroyComponent();
		m_MotionWarpingComponent = nullptr;
	}
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_AnimInstance = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (m_AnimInstance != nullptr)
	{


	}
	else
	{
		// (추측) CastChecked를 했었지만... 에디터에서 생성할때는 아마도 프로토 타입을 만들때는 GetMesh가 없어서 그때는 터지는 거같다..
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ACharacterBase::SetCharacterControlData(const UCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

}

void ACharacterBase::ShoulderMove(const FVector2D& Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Value.X);
	AddMovementInput(RightDirection, Value.Y);
}

void ACharacterBase::ShoulderLook(const FVector2D& Value)
{
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ACharacterBase::QuaterMove(FVector2D Value)
{
	float InputSizeSquared = Value.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = Value.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		Value.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Value.X, Value.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void ACharacterBase::WhealInput(const float& Value)
{

}

void ACharacterBase::RotateToInterp(FVector2D Value, float InterpSpeed)
{
	// 입력이 없으면 회전하지 않음
	if (Value.IsNearlyZero())
	{
		return;
	}

	//// 정규화
	//Value.Normalize();

	// 입력 벡터 → 월드 방향
	FVector MoveDirection = FVector(Value.X, Value.Y, 0.0f);

	//// 목표 회전
	//FRotator TargetRot = FRotationMatrix::MakeFromX(MoveDirection).Rotator();
	//
	//// 현재 회전
	//FRotator CurrentRot = GetActorRotation();
	//
	//// 부드럽게 보간
	//FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), InterpSpeed);

	// 적용
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
}

void ACharacterBase::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ACharacterBase::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}

void ACharacterBase::SetSprint(float Ratio)
{
	if (Ratio <= 0.f)
		return;

	GetCharacterMovement()->MaxWalkSpeed *= Ratio;
}

ECollisionChannel ACharacterBase::GetFriendlyCollisionTeambyChannel()
{
	// ECC_GameTraceChannel1이 14라서 더해주고 있다. 
	return ECollisionChannel(uint8(m_CollisionTeamData->FriendyCollision) + uint8(CChannel::JWPlayerAttack));
}

ECollisionChannel ACharacterBase::GetNeutralCollisionTeambyChannel()
{
	// ECC_GameTraceChannel1이 14라서 더해주고 있다. 
	return ECollisionChannel(uint8(m_CollisionTeamData->NeutralCollision) + uint8(CChannel::JWPlayerAttack));
}

ECollisionChannel ACharacterBase::GetHostileCollisionTeambyChannel()
{
	// ECC_GameTraceChannel1이 14라서 더해주고 있다. 
	return ECollisionChannel(uint8(m_CollisionTeamData->HostileCollision) + uint8(CChannel::JWPlayerAttack));
}

void ACharacterBase::ToggleGhostTrail()
{
	if (m_GhostTrailNiagaraSystem != nullptr)
	{
		m_GhostTrailNiagaraSystem->ToggleActive();
	}
}

bool ACharacterBase::IsActiveGhostTrail()
{
	if (m_GhostTrailNiagaraSystem != nullptr)
	{
		return m_GhostTrailNiagaraSystem->IsActive();
	}
	return false;
}

void ACharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadMontage();
	SetActorEnableCollision(false);
}

void ACharacterBase::PlayDeadMontage()
{
	m_AnimInstance->StopAllMontages(0.0f);
	m_AnimInstance->PlayDeadMontage(1.0f);
}


