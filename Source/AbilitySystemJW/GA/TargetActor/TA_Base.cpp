// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TargetActor/TA_Base.h"
#include "AbilitySystemComponent.h"
#include "Actor/AOEBase.h"

ATA_Base::ATA_Base() :
	m_ConfirmationType(EGameplayTargetingConfirmation::Instant)
{
}

void ATA_Base::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	if (m_ConfirmationType == EGameplayTargetingConfirmation::UserConfirmed)
	{
		BindToConfirmCancelInputs();
	}

	if (m_WorldRecticleClass != nullptr)
	{
		// World Recticle Section
		m_WorldRecticle = GetWorld()->SpawnActorDeferred<AAOEBase>(m_WorldRecticleClass, FTransform::Identity, SourceActor, CastChecked<APawn>(SourceActor), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		FVector SpawnLocation = SourceActor->GetActorLocation();// +SourceActor->GetActorForwardVector();
		FTransform SpawnTransform(SourceActor->GetActorRotation(), SpawnLocation);
		m_WorldRecticle->FinishSpawning(SpawnTransform);
	}
}

void ATA_Base::ConfirmTargetingAndContinue()
{
	// 호출하게 되면 충돌이 일어 날 수 있다. 커스텀 타겟데이터를 만들때는 안해준다.  
	//Super::ConfirmTargetingAndContinue();

		// 월드 레티클 제거.
	if (m_WorldRecticle != nullptr)
	{
		m_WorldRecticle->Destroy();
		m_WorldRecticle = nullptr;
	}
}

FGameplayAbilityTargetDataHandle ATA_Base::MakeTargetData() const
{
	FGameplayAbilityTargetDataHandle Handle;
	return Handle;
}


FGameplayAbilityTargetDataHandle ATA_Base::MakeTargetData(const FHitResult& HitResult) const
{
	FGameplayAbilityTargetDataHandle Handle;
	return Handle;
}
