// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Defense.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceBase.h"
#include "Tag/JWGameplayTag.h"

#include "GA/TargetActor/TA_Trace.h"
#include "GA/Task/AT_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/CharacterAttributeSet.h"
#include "AbilitySystemJW.h"
#include "Header/JWHeader.h"
#include "Components/TargetSystemComponent.h"
#include "MotionWarpingComponent.h"
#include "FunctionLibrary/JWFunctionLibrary.h"

UGA_Defense::UGA_Defense()
	:m_IsDefending(false)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Defense::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());


	// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정
	ComputeMotionWarping(Character);

	m_IsDefending = true;

	m_PlayDefenseTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDefense"), AnimInstance->GetCurrentDefenseMontage(), 1.0f, GetNextSection());
	m_PlayDefenseTask->OnCompleted.AddDynamic(this, &UGA_Defense::OnCompleteCallback);
	m_PlayDefenseTask->OnInterrupted.AddDynamic(this, &UGA_Defense::OnInterruptedCallback);
	m_PlayDefenseTask->ReadyForActivation();

	m_WaitHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, JWTAG_SHARED_EVENT_HITREACT, nullptr, false, true);
	m_WaitHitEventTask->EventReceived.AddDynamic(this, &UGA_Defense::OnHitEventCallback);
	m_WaitHitEventTask->ReadyForActivation();
}

void UGA_Defense::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (m_IsDefending == true)
	{
		// 여기서 방어중일때 패링 모션이 나올지 방어 해제 모션이 나올지를 정해야한다 .
		m_IsDefending = false;
		//ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
		//UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());

		const FVector& OwnerLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
		const FRotator& OwnerRotation = CurrentActorInfo->AvatarActor->GetActorRotation();
		m_ParryTraceTask = UAT_Trace::CreateTask(this, m_TargetActorClass);
		m_ParryTraceTask->OnComplete.AddDynamic(this, &UGA_Defense::OnTraceResultCallback);
		m_ParryTraceTask->ReadyForActivation();

	}
	else
	{
		return;
	}
}

void UGA_Defense::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UGA_Defense::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 몽타주 정지.
	MontageStop(0.25f);
	
	if (IsValid(m_PlayDefenseTask) == true)
	{
		m_PlayDefenseTask->EndTask();
		m_PlayDefenseTask = nullptr;
	}

	if(IsValid(m_WaitHitEventTask) == true)
	{
		m_WaitHitEventTask->EndTask();
		m_WaitHitEventTask = nullptr;
	}

	if (IsValid(m_ParryTraceTask) == true)
	{
		m_ParryTraceTask->EndTask();
		m_ParryTraceTask = nullptr;
	}


	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor);
	UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();
	if (MotionWarpingComp != nullptr)
	{
		MotionWarpingComp->RemoveWarpTarget(FName(TEXT("TargetActor")));
	}
}

void UGA_Defense::OnCompleteCallback()
{
	//JWGAS_LOG(LogJWGAS, Log, TEXT("CurrentShootState : %d , Time : %lf"), m_ShootState,GetWorld()->GetRealTimeSeconds());
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

void UGA_Defense::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
}

void UGA_Defense::OnHitEventCallback(FGameplayEventData Payload)
{
	// Damage Section


	// Montage Section 
	MontageJumpToSection(FName(TEXT("Damage")));
	MontageSetNextSectionName(FName(TEXT("Damage")), FName(TEXT("Loop")));
}

FName UGA_Defense::GetNextSection()
{
	return FName(TEXT("Start"));
}

void UGA_Defense::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		// HitResult가 있는 데이터 

		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		JWGAS_LOG(LogJWGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Ensured();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (SourceASC == nullptr || TargetASC == nullptr)
		{
			JWGAS_LOG(LogJWGAS, Error, TEXT("ASC not found!"));
			return;
		}

		const UCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UCharacterAttributeSet>();
		if (SourceAttribute == nullptr)
		{
			JWGAS_LOG(LogJWGAS, Error, TEXT("Attribute not found!"));
			return;
		}

		if (TargetASC->HasMatchingGameplayTag(JWTAG_CHARACTER_ALLOWPARRY))
		{
			// (임시) 임시로 이벤트를 발생시키는중 .  
			//FGameplayEventData PayloadData;
			//PayloadData.EventMagnitude = 1.0f;
			//PayloadData.Instigator = Character;
			//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), JWTAG_EVENT_CHARACTER_STUN, PayloadData);
			for (auto EffectClass : m_ParryDamageEffects_Self)
			{
				// Parry Effect Part
				FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
				if (EffectSpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
					//FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
					//CueContextHandle.AddHitResult(HitResult);
					//FGameplayCueParameters CueParam;
					//CueParam.EffectContext = CueContextHandle;
					//
					//TargetASC->ExecuteGameplayCue(JWTAG_GAMEPLAYCUE_CHARACTER_PARRYHIT, CueParam);
				}
			}

			for (auto EffectClass : m_ParryDamageEffects_Target)
			{
				// Parry Effect Part
				FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass);
				if (EffectSpecHandle.IsValid())
				{
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
					//FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
					//CueContextHandle.AddHitResult(HitResult);
					//FGameplayCueParameters CueParam;
					//CueParam.EffectContext = CueContextHandle;
					//
					//TargetASC->ExecuteGameplayCue(JWTAG_GAMEPLAYCUE_CHARACTER_PARRYHIT, CueParam);
				}
			}


			Debug::Print(TEXT("Parry Success!"), FColor::Green);

			// Play Animation
			MontageJumpToSection(FName(TEXT("Parry")));
		}
		else
		{
			// Play Animation
			MontageJumpToSection(FName(TEXT("End")));
		}

	}
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		// HitResult가 없는 데이터 

		// Play Animation
		MontageJumpToSection(FName(TEXT("End")));
	}
	else
	{
		// TargetActor is Null
		
		// Play Animation
		//MontageSetNextSectionName(FName(TEXT("Loop")), FName(TEXT("End")));
		MontageJumpToSection(FName(TEXT("End")));
	}
}

void UGA_Defense::ComputeMotionWarping(ACharacter* Character)
{
	// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정
	// (기록) 루트모션을 활성화한 몽타주는 모든 이동이 제한된다. (루트모션이 활성화 되면서 활성화 하지 않은 몽타주내 다른 시퀀스도 영향을 받는듯하다. ) 
	const UTargetSystemComponent* TargetSystemComp = Character->FindComponentByClass<UTargetSystemComponent>();
	if(TargetSystemComp == nullptr) return;
	UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();
	if (MotionWarpingComp == nullptr) return;
	
	AActor* TargetActor = TargetSystemComp->GetTargetActor();
	// 지금 까지 ACharacterBase로 자식단 까지 가져와서 사용했는데 그럴 필요가 없었다.... [ 언리얼의 구조를 잘못 이해하고 있어서 생긴 문제 ... ] 
	if (TargetActor != nullptr)
	{
		FVector Direction = (TargetActor->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
		Direction.Z = 0.0;
		
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("TargetActor")), Character->GetActorLocation(), Direction.Rotation());
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

	}
	
}