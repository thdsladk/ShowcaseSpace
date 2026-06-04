// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Attack.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimInstanceBase.h"
#include "Components/CapsuleComponent.h"

#include "Data/ComboActionData.h"
#include "MotionWarpingComponent.h"
#include "CharacterComponents/TargetSystemComponent.h"
#include "Header/JWHeader.h"

#include "FunctionLibrary/JWFunctionLibrary.h"

/*
	Player 전용 어빌리티. 
*/

UGA_Attack::UGA_Attack()
	:m_CurrentComboData(nullptr), m_CurrentCombo(0), m_ComboTimerHandle()
	, m_HasNextComboInput(false), m_OriginMovementMode(0)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
	m_CurrentComboData = AnimInstance->GetCurrentComboActionData();


	// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정
	ComputeMotionWarping(Character);
	
	m_PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AnimInstance->GetCurrentAttackMontage(), 1.0f, GetNextSection());
	m_PlayAttackTask->OnCompleted.AddDynamic(this, &UGA_Attack::OnCompleteCallback);
	m_PlayAttackTask->OnInterrupted.AddDynamic(this, &UGA_Attack::OnInterruptedCallback);
	m_PlayAttackTask->ReadyForActivation();

	StartComboTimer(); 
}

void UGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 타이머 종료를 기준으로 다음 콤보가 진행되는지 판단. 
	m_HasNextComboInput = m_ComboTimerHandle.IsValid();
}

void UGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	// 몽타주 정지.
	MontageStop(0.25f);

	m_CurrentComboData = nullptr;
	m_CurrentCombo = 0;
	m_HasNextComboInput = false;

	if (m_PlayAttackTask != nullptr)
	{
		m_PlayAttackTask->EndTask();
		m_PlayAttackTask = nullptr;
	}

	// MotionWarping 타겟 제거
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor);
	UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();
	if (MotionWarpingComp != nullptr)
	{
		MotionWarpingComp->RemoveWarpTarget(FName(TEXT("TargetActor")));
		MotionWarpingComp->RemoveWarpTarget(FName(TEXT("MovePos")));
	}

	// 콤보 타이머 초기화
	m_ComboTimerHandle.Invalidate();
}

void UGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
}

FName UGA_Attack::GetNextSection()
{
	m_CurrentCombo = FMath::Clamp(m_CurrentCombo + 1, 1, m_CurrentComboData->m_MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *m_CurrentComboData->m_MontageSectionNamePrefix, m_CurrentCombo);
	return NextSection;
}

void UGA_Attack::StartComboTimer()
{
	int32 ComboIndex = m_CurrentCombo - 1;
	ensure(m_CurrentComboData->m_EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float ComboEffectiveTime = m_CurrentComboData->m_EffectiveFrameCount[ComboIndex] / m_CurrentComboData->m_FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(m_ComboTimerHandle, this, &UGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UGA_Attack::CheckComboInput()
{
	m_ComboTimerHandle.Invalidate();
	if (m_HasNextComboInput == true)
	{
		// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정
		ACharacter* Character = CastChecked<ACharacter>(GetAbilitySystemComponentFromActorInfo_Ensured()->GetAvatarActor());
		ComputeMotionWarping(Character);

		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		m_HasNextComboInput = false;
	}

}

void UGA_Attack::ComputeMotionWarping(ACharacter* Character)
{
	// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정

	if (Character != nullptr)
	{
		const UTargetSystemComponent* TargetSystemComp = Character->FindComponentByClass<UTargetSystemComponent>();
		if (TargetSystemComp == nullptr) return;
		UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();

		AActor* TargetActor = TargetSystemComp->GetAutoTarget();
		const float MaxDistance = 300.f;
		float MinDistance;
		FVector Direction;


		if (MotionWarpingComp != nullptr)
		{
			if (TargetActor != nullptr)
			{
				Direction = (TargetActor->GetActorLocation() - Character->GetActorLocation());
				UCapsuleComponent* TargetCapsule = TargetActor->FindComponentByClass<UCapsuleComponent>();
				UCapsuleComponent* SelfCapsule = Character->FindComponentByClass<UCapsuleComponent>();
				if (TargetCapsule != nullptr && SelfCapsule != nullptr)
				{
					MinDistance = TargetCapsule->GetScaledCapsuleRadius() + SelfCapsule->GetScaledCapsuleRadius();
				}
				float Distance = Direction.Length() - MinDistance;// 50.f 는 겹치지 않게 하기 위한 거리.
				Direction = Direction.GetSafeNormal();
				Distance = FMath::Clamp(Distance, MinDistance, MaxDistance);
				Direction.Z = 0.0;

				MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("TargetActor")), Character->GetActorLocation(), Direction.Rotation());
				MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(FName(TEXT("MovePos")), Character->GetActorLocation() + (Direction * Distance));
			}
			else
			{
				// 타겟이 없으면 마우스 커서 방향으로 회전
				APlayerController* PC = Cast<APlayerController>(Character->GetController());
				if (PC != nullptr)
				{
					FRotator NewRotator = UJWFunctionLibrary::ComputeLookAtRotationFromMouse(PC);
					MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("TargetActor")), Character->GetActorLocation(), NewRotator);
				}

				// 디버그 시각화
				//DrawDebugSphere(GetWorld(), Character->GetActorLocation() + (Character->GetActorForwardVector() * MaxDistance), 25.f, 12, FColor::Red, false, 2.f);
				//DrawDebugLine(GetWorld(), Character->GetActorLocation() + (Character->GetActorForwardVector() * MaxDistance), Character->GetActorLocation() + (Character->GetActorForwardVector() * MaxDistance) + Character->GetActorForwardVector() * 100.f, FColor::Blue, false, 2.f, 0, 2.f);
				//Debug::Print((Character->GetActorForwardVector() * MaxDistance).ToString(), FColor::Green);
			}
		}
		else
		{
			if (TargetActor != nullptr)
			{
				// 여기서 TargetActor를 세팅해 줘야한다.
				Direction = (TargetActor->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
				Character->SetActorRotation(Direction.Rotation());
				Character->GetController()->SetControlRotation(Direction.Rotation());
			}
			else
			{
				// TargetActor가 없으면 실행 못한다. 
			}
			
		}
		
	}

}
