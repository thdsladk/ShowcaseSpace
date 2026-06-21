// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor/TA_Base.h"
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

	if(m_ScopeRecticle != nullptr)
	{
		m_ScopeRecticle->Destroy();
		m_ScopeRecticle = nullptr;
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
