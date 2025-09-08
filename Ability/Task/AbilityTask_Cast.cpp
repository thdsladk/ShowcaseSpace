// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Task/AbilityTask_Cast.h"
#include "MyAbilityInstance.h"
#include "Header\DataStruct.h"
#include "Projectile/ProjectileBase.h"

void UAbilityTask_Cast::Start(UMyAbilityInstance* Owner)
{
	m_pOwner = Owner;
	if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveForwardTask Start failed: Invalid owner"));
		Cancel();
		return;
	}


	if (m_ProjectileClass != nullptr)
	{
		AProjectileBase* Projectile = Owner->GetWorld()->SpawnActor<AProjectileBase>(m_ProjectileClass, Owner->GetAbilityComponent()->GetOwner()->GetActorLocation(), Owner->GetAbilityComponent()->GetOwner()->GetActorRotation());
	}

}

void UAbilityTask_Cast::Tick(float DeltaTime)
{
	if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
	{
		Cancel();
		return;
	}

	m_ElapsedTime += DeltaTime;
	if (m_ElapsedTime >= m_Duration)
	{
		
		Cancel();
		return;
	}
}

void UAbilityTask_Cast::Cancel()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner = nullptr;
		m_OnCompleted.Broadcast();
	}
	else
	{
		return;
	}
}

bool UAbilityTask_Cast::WantsTick() const
{
	return false;
}
