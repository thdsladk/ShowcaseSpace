// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ActionAbility/GA_ActionAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "CharacterComponents/LinkedActionComponent.h"
#include "Character/CharacterBase.h"


UGA_ActionAbilityBase::UGA_ActionAbilityBase()
	: m_IsActive(false)
{
}

void UGA_ActionAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Cost와 Cooldown을 사용하려면 호출 필수 
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	if (Spec != nullptr)
	{
		m_InputID = Spec->InputID;
	}


	// 이부분은 자식에서 정의 되어도 될듯하다. 
	//m_PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDefense"), m_DefaultMontage, 1.0f, m_SectionName);
	//m_PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_ActionAbilityBase::OnCompleteCallback_Montage);
	//m_PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_ActionAbilityBase::OnInterruptedCallback_Montage);
	//m_PlayMontageTask->ReadyForActivation();

}

void UGA_ActionAbilityBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_ActionAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}

void UGA_ActionAbilityBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{


}

void UGA_ActionAbilityBase::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{


}

void UGA_ActionAbilityBase::OnCompleteCallback_Montage()
{
}

void UGA_ActionAbilityBase::OnInterruptedCallback_Montage()
{
}

void UGA_ActionAbilityBase::PlayLinkedAction()
{
	// (임시) 아직은 컴포넌트들을 얻어오는 인터페이스가 없어서 CharcterBase로 캐스팅 하고있다. 
	ACharacterBase* Character = CastChecked<ACharacterBase>(GetAvatarActorFromActorInfo());
	
	Character->GetLinkedActionComp()->EndAction(m_InputID);
}
