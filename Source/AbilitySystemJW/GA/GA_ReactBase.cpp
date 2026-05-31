// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_ReactBase.h"
#include "AbilitySystemJW.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Tag/JWGameplayTag.h"

#include "MotionWarpingComponent.h"

#include "Interface/PlayerControllerInterface.h"



UGA_ReactBase::UGA_ReactBase()
	:m_CurrentLevel(1.0f)
{
}

void UGA_ReactBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(Character->GetController());

	// 모션 워핑 
	ComputeMotionWarping(Character, *TriggerEventData->Instigator);


	UAbilitySystemComponent* SelfASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	
	// 캐릭터의 행동 상태에 따라서 리액트 애니메이션을 다르게 재생
	UAnimMontage* Montage = nullptr;
	if (SelfASC->HasMatchingGameplayTag(JWTAG_CHARACTER_ISDEFENSE) == true)
	{
		// 방어 상태
		if (PCI != nullptr)	// 플레이어의 캐릭터 일때
		{
			Montage = m_DefenseReactMontage.Find(ECombatMode(PCI->GetCombatMode()))->Get();
		}
	}
	else
	{
		// 아무런 상태도 없는 Idle일때 
		Montage = m_Montage;
	}

	// 공격 타입을 기반으로 리액트 애니메이션 섹션을 추출
	if (TriggerEventData != nullptr && TriggerEventData->TargetTags.IsEmpty() == false)
	{
		for (auto Tag : TriggerEventData->TargetTags)
		{
			FString SectionName;
			FGameplayTag ParentTag = FGameplayTag::RequestGameplayTag("Event");
			if (Tag.MatchesTag(ParentTag) == true)
			{
				FString FullTagName = Tag.ToString();

				int32 LastDotIndex = 0;
				if (FullTagName.FindLastChar('.', LastDotIndex))
				{
					SectionName = FullTagName.RightChop(LastDotIndex + 1);
					UE_LOG(LogTemp, Log, TEXT("Last Tag Part: %s"), *SectionName);

					m_SectionNameMap.Add(Tag, *SectionName);
				}

				// Event 태그를 하나만 확인하고 break;
				m_ReactTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayReact"), Montage, 1.0f, *SectionName);
				m_ReactTask->OnCompleted.AddDynamic(this, &UGA_ReactBase::OnCompleteCallback);
				m_ReactTask->OnInterrupted.AddDynamic(this, &UGA_ReactBase::OnInterruptedCallback);
				m_ReactTask->ReadyForActivation();

				// (임시) 이건 아직 하드 코딩인거 같다 특정 상태를 임의로 코드에서 세팅해주고 있다. 
				if (SectionName.Compare(TEXT("Stun")) == 0)
				{
					m_TagRemovedTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, JWTAG_CHARACTER_ISSTUN);
					m_TagRemovedTask->Removed.AddDynamic(this, &UGA_ReactBase::OnRemovedTagCallback);
					m_TagRemovedTask->ReadyForActivation();

				}


				break;

			}
			else
			{
				// Event 태그가 아니면 넘기기.
				continue;
			}
		}
	}
}

void UGA_ReactBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UGA_ReactBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 몽타주 정지.
	MontageStop(0.2f);

	if (m_ReactTask != nullptr && m_ReactTask->IsActive() ==true)
	{
		m_ReactTask->EndTask();
		m_ReactTask = nullptr;
	}

	if (m_TagRemovedTask != nullptr && m_TagRemovedTask->IsActive() == true)
	{
		m_TagRemovedTask->EndTask();
		m_TagRemovedTask = nullptr;
	}
}

void UGA_ReactBase::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_ReactBase::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
}

void UGA_ReactBase::OnRemovedTagCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_ReactBase::ComputeMotionWarping(ACharacter* Character, const AActor& Instigator)
{
	UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();

	// 모션 워핑으로 다시 방향이랑 넉백을 구현했는데 [ 특정 애니메이션들은 루트모션이 없어서 안된다. ] 
// 정규화의 순서가 중요하다. Z값을 없애고 정규화를 해야 정확한 XY기반의 반향벡터가 나올 수 있다.

	FVector Direction = (Instigator.GetActorLocation() - Character->GetActorLocation());
	Direction.Z = 0.0;
	Direction = Direction.GetSafeNormal();


	// 넉백    (임시)  미는 크기를 하드코딩했다.  
	FVector Velocity = -(Direction * m_Force);
	// 높이를 없애주자.
	Velocity.Z = 0.0f;
	FVector Position = Character->GetActorLocation() + Velocity;
	if (MotionWarpingComp != nullptr && m_Montage->HasRootMotion() == true)
	{
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("KnockbackRotation")), FVector(), Direction.Rotation());
		MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("KnockbackPosition")), Position, Direction.Rotation());
	}
	else    // 모션워핑이 존재하지 않으면
	{
		Character->SetActorRotation(Direction.Rotation());
		Character->GetController()->SetControlRotation(Direction.Rotation());

		Character->LaunchCharacter(Velocity, true, true);	
		// 이거는 물리 시뮬레이트를 켜야하는데 제어가 불편하다. 네비메시를 사용중이라서 충돌한다. 
		//Character->GetCharacterMovement()->AddImpulse(Velocity, false);
	}

}