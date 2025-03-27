// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterBase::ACharacterBase() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Combat Stat    // ���߿� �����͸� �޾Ƽ� �ʱ�ȭ �� ���̶� 0.f�� �ʱ�ȭ
	m_AttackRange = 0.f;
	m_AttackRadius = 0.f;
	m_DefenseRadius = 0.f;
	m_DetectionRadius = 0.f;
	m_VisibleRadius = 0.f;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 800.f; // 600.f;

	// Speed Set ( MaxWalkSpeed : 600.f )
	m_OriginMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;


	// Set Components
	//m_pStatComp = CreateDefaultSubobject<UMyStatComponent>(TEXT("Stat"));


	static ConstructorHelpers::FObjectFinder<UMaterial> Shiny(TEXT("/Game/TopDown/Material/M_Shiny.M_Shiny"));
	if (Shiny.Succeeded())
	{
		m_Shiny = Shiny.Object;
	}


}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	

	// OriginMaterial Init Setting
	m_OriginMaterial = GetMesh()->GetMaterial(0);






}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::Effect_Flick(FLinearColor Color)
{
	if (true == GetWorld()->GetTimerManager().IsTimerActive(m_hEffectFlickTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer(m_hEffectFlickTimer, this, &ACharacterBase::EffectEnd_Flick, 0.1f, false);

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(m_Shiny, this);
	DynamicMaterial->SetVectorParameterValue(FName("Param_1"), Color);

	GetMesh()->SetMaterial(0, DynamicMaterial);

	//DEBUG

	//bool OK = false;
	//if (DynamicMaterial->IsVectorParameterUsedAsChannelMask(TEXT("Param_1"), OK) == true)
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : OK ")); }
	//else 
	//{ UE_LOG(LogTemp, Log, TEXT("ColorName : Failed ")); }
}

void ACharacterBase::EffectEnd_Flick()
{
	GetMesh()->SetMaterial(0, m_OriginMaterial);
}

void ACharacterBase::MoveToFoward(float Value)
{
	// ���� ������ �ִϸ��̼� ������ ������ ���� ������
	m_Vertical = FMath::Clamp(Value, -1.f, 1.f);
	AddMovementInput(GetActorForwardVector(), Value);
}

void ACharacterBase::MoveToRight(float Value)
{
	// ���� ������ �ִϸ��̼� ������ ������ ���� ������
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);
	AddMovementInput(GetActorRightVector(), Value);
}

void ACharacterBase::SetMoveSpeed(float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// ĳ���� �̵��ӵ�
}

void ACharacterBase::ResetMoveSpeed()
{
	// �����ε带 �����ʰ� ��������� Reset�̶�� �ܾ�� �������� ����Ͽ���.
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;
}

void ACharacterBase::SetMoveSpeed_Ratio(float Ratio)
{	
	// ���� 0.f~100.f���� �����ϱ� ���ؼ� Clamp���ش�.
	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed * FMath::Clamp(Ratio, 0.f, 100.f);
}

void ACharacterBase::Sprint()
{
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;			// ĳ���� �̵��ӵ�	(�ӽ�) ����� �ٲ���Ѵ�.
	}
}

void ACharacterBase::SetSprint(float WalkSpeed, float CameraSpeed)
{
	// ACharacterBase�� ī�޶� ���� ������ ������� �ʴ´�. 
	if (m_CharacterState == EBehaviorState::Idle || m_CharacterState == EBehaviorState::Battle)
	{
		m_CharacterState = EBehaviorState::Running;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;		// ĳ���� �̵��ӵ�
	}
}

void ACharacterBase::StopSprint()
{
	if (m_Mode == ECharacterMode::Idle)
	{
		m_CharacterState = EBehaviorState::Idle;
	}
	else if(m_Mode == ECharacterMode::Battle)
	{
		m_CharacterState = EBehaviorState::Battle;
	}

	GetCharacterMovement()->MaxWalkSpeed = m_OriginMoveSpeed;		// ĳ���� �̵��ӵ�
}

void ACharacterBase::OnIdle()
{
}

void ACharacterBase::OnBattle()
{
}

void ACharacterBase::Attack()
{
}

void ACharacterBase::AttackCheck()
{
}

void ACharacterBase::Death()
{
}

void ACharacterBase::DeathEnd()
{
}

void ACharacterBase::OnHit()
{
}

void ACharacterBase::AttackEnd()
{
}

void ACharacterBase::OnDefense()
{
}

void ACharacterBase::StopDefense()
{
}

void ACharacterBase::Defense_Hit()
{
}

void ACharacterBase::OnDetect()
{
}

void ACharacterBase::OnAlert()
{
}

void ACharacterBase::OnCommand(uint8 Command)
{
}

void ACharacterBase::LookDirection(float Value)
{
	// ���� ������ �ִϸ��̼� ������ ������ ���� ������
	m_Horizontal = FMath::Clamp(Value, -1.f, 1.f);

	//Debug
	//UE_LOG(LogTemp, Log, TEXT("Horizontal : %f "), m_Horizontal);
}

void ACharacterBase::SetUpdateBehaviorDelegate(const FOnUpdateBehavior& Delegate)
{
}

bool ACharacterBase::IsOnTriggerEscape()
{
	return false;
}
