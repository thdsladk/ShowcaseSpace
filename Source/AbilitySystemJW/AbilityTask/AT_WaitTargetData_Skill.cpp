// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask/AT_WaitTargetData_Skill.h"
#include "AbilitySystemComponent.h"
#include "TargetActor/TA_Skill.h"
#include "WorldReticle/WR_Base.h"
#include "GameFramework/Character.h"
#include "CharacterComponents/TargetSystemComponent.h"

#include "Header/GASStructTypes.h"


UAT_WaitTargetData_Skill::UAT_WaitTargetData_Skill() :
	m_SpawnedTargetActor(nullptr)
{
}

UAT_WaitTargetData_Skill* UAT_WaitTargetData_Skill::WaitTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType, TSubclassOf<ATA_Skill> InTargetClass)
{
	UAT_WaitTargetData_Skill* MyObj = NewAbilityTask<UAT_WaitTargetData_Skill>(OwningAbility, TaskInstanceName);		//Register for task list here, providing a given FName as a key
	MyObj->TargetClass = InTargetClass;
	MyObj->TargetActor = nullptr;
	// 스킬 판정은 체크없이 진행하도록 하자. 
	//MyObj->ConfirmationType = ConfirmationType;
	MyObj->ConfirmationType = EGameplayTargetingConfirmation::Instant;

	return MyObj;
}

void UAT_WaitTargetData_Skill::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();
	SetWaitingOnAvatar();
}

void UAT_WaitTargetData_Skill::OnDestroy(bool AbilityEnded)
{
	if (m_SpawnedTargetActor != nullptr)
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->SpawnedTargetActors.Remove(m_SpawnedTargetActor);
		}
		m_SpawnedTargetActor->Destroy();
	}
	Super::OnDestroy(AbilityEnded);
}

void UAT_WaitTargetData_Skill::SpawnAndInitializeTargetActor()
{
	AActor* OwningActor = Ability->GetAvatarActorFromActorInfo();
	if (OwningActor == nullptr) return;

	m_SpawnedTargetActor = Cast<ATA_Skill>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetClass, FTransform::Identity, OwningActor, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (m_SpawnedTargetActor != nullptr)
	{
		//m_SpawnedTargetActor->SetShowDebug(false);
		m_SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_WaitTargetData_Skill::OnTargetDataReadyCallback);
		m_SpawnedTargetActor->SetConfirmationType(ConfirmationType);
	}
	else
	{
		OnTargetDataReadyCallback(FGameplayAbilityTargetDataHandle());
	}
}

void UAT_WaitTargetData_Skill::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC != nullptr)
	{
		if (m_SpawnedTargetActor != nullptr)

		{
			const FTransform& SpawnTransform = ASC->GetAvatarActor()->GetTransform();
			m_SpawnedTargetActor->FinishSpawning(SpawnTransform);
			ASC->SpawnedTargetActors.Push(m_SpawnedTargetActor);
			m_SpawnedTargetActor->StartTargeting(Ability);
			if (ConfirmationType == EGameplayTargetingConfirmation::Instant)
			{
				// 만약에 즉시 발동하고 타겟 액터가 더이상 필요 없으면 지금 처럼인데 Instant의 정의가 타겟액터의 위치값이랑 방향등 데이터를 안써도 된다면 바로 가능할듯하다. 
				m_SpawnedTargetActor->ConfirmTargeting();
			}
		}
	}
}

void UAT_WaitTargetData_Skill::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates() == true)
	{
		OnComplete.Broadcast(DataHandle);
	}
	EndTask();
}
