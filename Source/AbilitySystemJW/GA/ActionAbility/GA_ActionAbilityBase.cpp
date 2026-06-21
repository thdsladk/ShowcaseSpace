// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ActionAbility/GA_ActionAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "CharacterComponents/LinkedActionComponent.h"
#include "GameFramework/Character.h"

#include "FunctionLibrary/JWFunctionLibrary.h"


UGA_ActionAbilityBase::UGA_ActionAbilityBase()
	: m_IsActive(false)
{
}

void UGA_ActionAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 디버그 모드에서는 쿨다운이랑 비용이 안들도록 세팅
	if (UJWFunctionLibrary::IsDebugMode(ActorInfo->AvatarActor.Get()) == false)
	{
		// Cost와 Cooldown을 사용하려면 호출 필수 
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
	FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	if (Spec != nullptr)
	{
		m_InputID = Spec->InputID;
	}

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
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	ULinkedActionComponent* LinkedActionComp = Character->FindComponentByClass<ULinkedActionComponent>();
	LinkedActionComp->EndAction(m_InputID);
}
