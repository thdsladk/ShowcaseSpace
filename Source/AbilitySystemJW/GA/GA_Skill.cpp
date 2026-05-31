// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Skill.h"
#include "Character/GASPlayerCharacter.h"
#include "Animation/AnimInstanceBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities\Tasks\AbilityTask_PlayMontageAndWait.h"

UGA_Skill::UGA_Skill()
{
}

void UGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AGASPlayerCharacter* TargetCharacter = CastChecked<AGASPlayerCharacter>(ActorInfo->AvatarActor.Get());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(TargetCharacter->GetMesh()->GetAnimInstance());
	
	
	m_ActiveSkillActionMontage = AnimInstance->GetSkillActionMontage();
	if (m_ActiveSkillActionMontage == nullptr)
	{
		return;
	}

	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), m_ActiveSkillActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Skill::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();
}

void UGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AGASPlayerCharacter* TargetCharacter = CastChecked<AGASPlayerCharacter>(ActorInfo->AvatarActor.Get());
	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Skill::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

void UGA_Skill::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}