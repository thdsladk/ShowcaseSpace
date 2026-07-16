// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstanceBase.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/PlayerControllerInterface.h"
#include "KismetAnimationLibrary.h"


UAnimInstanceBase::UAnimInstanceBase()
{
	m_MovingThreshould = 3.0f;
	m_JumpingThreshould = 100.0f;
	m_ViewAngle = 90.f;

	m_CombatModeRef = ECombatMode::IdleMode;

	m_CombatModeMontages.Reset(uint8(ECombatMode::End));
}

void UAnimInstanceBase::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);
	GameplayTagPropertyMap.Initialize(this, ASC);
}

EDataValidationResult UAnimInstanceBase::IsDataValid(FDataValidationContext& Context) const
{
	// 부모 클래스 검증 먼저 실행
	EDataValidationResult Result = Super::IsDataValid(Context);

	// GameplayTagPropertyMap 검증
	EDataValidationResult TagResult = GameplayTagPropertyMap.IsDataValid(this,Context);

	// 최종 결과는 두 결과 중 더 심각한 수준을 반환
	if (Result == EDataValidationResult::Invalid || TagResult == EDataValidationResult::Invalid)
	{
		return EDataValidationResult::Invalid;
	}
	if (Result == EDataValidationResult::Valid && TagResult == EDataValidationResult::Valid)
	{
		return EDataValidationResult::Valid;
	}
	return EDataValidationResult::NotValidated;
}

void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_Owner = Cast<ACharacter>(GetOwningActor());
	if (m_Owner != nullptr)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(m_Owner))
		{
			InitializeWithAbilitySystem(ASC);
		}
		m_Movement = m_Owner->GetCharacterMovement();
	}
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (m_Movement != nullptr)
	{
		m_Velocity = m_Movement->Velocity;
		m_GroundSpeed = m_Velocity.Size2D();
		m_bIsIdle = m_GroundSpeed < m_MovingThreshould;
		m_bIsFalling = m_Movement->IsFalling();
		m_bIsJumping = m_bIsFalling & (m_Velocity.Z > m_JumpingThreshould);

		m_Direction = UKismetAnimationLibrary::CalculateDirection(m_Velocity, m_Owner->GetActorRotation());
	}
}

void UAnimInstanceBase::PlayDeadMontage(float Rate)
{
	Montage_Play(m_DeadMontage, Rate);
}

void UAnimInstanceBase::BindEndMove(FOnEndMove Delegate)
{
	// 이 방식은 델리게이트를 그대로 복사하는 방식.
	OnEndMove = Delegate;
}

void UAnimInstanceBase::BindEndSection(FOnEndSection Delegate)
{
	// 이 방식은 델리게이트를 그대로 복사하는 방식.
	OnEndSection = Delegate;
}

void UAnimInstanceBase::BindShootPoint(FOnShootPoint Delegate)
{
	// 이 방식은 델리게이트를 그대로 복사하는 방식.
	OnShootPoint = Delegate;
}

void UAnimInstanceBase::BindNotifyPoint(FOnNotifyPoint Delegate)
{
	// 이 방식은 델리게이트를 그대로 복사하는 방식.
	OnNotifyPoint = Delegate;
}
