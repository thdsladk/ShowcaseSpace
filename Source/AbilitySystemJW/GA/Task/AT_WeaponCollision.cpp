// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Task/AT_WeaponCollision.h"
#include "AbilitySystemComponent.h"
#include "GA/TargetActor/TA_Base.h"

UAT_WeaponCollision::UAT_WeaponCollision()
{
}

UAT_WeaponCollision* UAT_WeaponCollision::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_Base> TargetActorClass)
{
	UAT_WeaponCollision* NewTask = NewAbilityTask<UAT_WeaponCollision>(OwningAbility);
	NewTask->m_TargetActorClass = TargetActorClass;
	return NewTask;
}

void UAT_WeaponCollision::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();
	
	SetWaitingOnAvatar();
}

void UAT_WeaponCollision::OnDestroy(bool AbilityEnded)
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

void UAT_WeaponCollision::SpawnAndInitializeTargetActor()
{
	m_SpawnedTargetActor = Cast<ATA_Base>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(m_TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (m_SpawnedTargetActor != nullptr)
	{
		//m_SpawnedTargetActor->SetShowDebug(false);
		m_SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_WeaponCollision::OnTargetDataReadyCallback);
	}
	else
	{
		// 태스크를 종료합니다.
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnComplete.Broadcast(FGameplayAbilityTargetDataHandle());
		}
		EndTask();
	}
}

void UAT_WeaponCollision::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC != nullptr)
	{
		if (m_SpawnedTargetActor != nullptr)
		{
			const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
			m_SpawnedTargetActor->FinishSpawning(SpawnTransform);

			ASC->SpawnedTargetActors.Push(m_SpawnedTargetActor);
			m_SpawnedTargetActor->StartTargeting(Ability);
		}
	}
}

void UAT_WeaponCollision::FinishWeaponTask(FGameplayEventData Payload)
{
	if (m_SpawnedTargetActor != nullptr)
	{
		m_SpawnedTargetActor->ConfirmTargeting();
	}

	// 태스크를 종료합니다.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(FGameplayAbilityTargetDataHandle());
	}
	EndTask();
}

void UAT_WeaponCollision::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnSendTargetData.Broadcast(DataHandle);
		//OnComplete.Broadcast(DataHandle);
	}

	//EndTask();
}