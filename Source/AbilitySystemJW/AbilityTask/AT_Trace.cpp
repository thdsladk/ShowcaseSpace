// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask/AT_Trace.h"
#include "AbilitySystemComponent.h"
#include "TargetActor/TA_Base.h"
#include "WorldReticle/WR_Base.h"

UAT_Trace::UAT_Trace() :
	m_SpawnedTargetActor(nullptr)
{
}

UAT_Trace* UAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_Base> TargetActorClass)
{
	UAT_Trace* NewTask = NewAbilityTask<UAT_Trace>(OwningAbility);
	NewTask->m_TargetActorClass = TargetActorClass;
	return NewTask;
}

void UAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();
	SetWaitingOnAvatar();
}

void UAT_Trace::OnDestroy(bool AbilityEnded)
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

void UAT_Trace::SpawnAndInitializeTargetActor()
{
	AActor* OwningActor = Ability->GetAvatarActorFromActorInfo();
	if (OwningActor == nullptr) return;
	
	m_SpawnedTargetActor = Cast<ATA_Base>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(m_TargetActorClass, FTransform::Identity, OwningActor, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (m_SpawnedTargetActor != nullptr)
	{
		//m_SpawnedTargetActor->SetShowDebug(false);
		m_SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_Trace::OnTargetDataReadyCallback);
	}
	else
	{
		OnTargetDataReadyCallback(FGameplayAbilityTargetDataHandle());
	}
}

void UAT_Trace::FinalizeTargetActor()
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
			m_SpawnedTargetActor->ConfirmTargeting();
		}
	}
}

void UAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
 	if (ShouldBroadcastAbilityTaskDelegates() == true)
	{
		OnComplete.Broadcast(DataHandle);
	}
	EndTask();
}