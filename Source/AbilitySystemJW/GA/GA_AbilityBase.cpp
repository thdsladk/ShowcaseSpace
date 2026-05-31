// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_AbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CharacterComponents/PawnCombatComponent.h"
#include "Animation/AnimInstanceBase.h"
#include "GameFramework/Character.h"


UPawnCombatComponent* UGA_AbilityBase::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UAbilitySystemComponent* UGA_AbilityBase::GetJWAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo->AbilitySystemComponent).Get();
}

UAnimInstanceBase* UGA_AbilityBase::GetAnimInstanceBaseFromActorInfo() const
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (AvatarActor != nullptr)
	{
		ACharacter* Character = Cast<ACharacter>(AvatarActor);
		if (Character != nullptr)
		{
			UAnimInstanceBase* AnimInstance = Cast<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
			if (AnimInstance != nullptr)
				return AnimInstance;
		}
	}
	return nullptr;
}

FActiveGameplayEffectHandle UGA_AbilityBase::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetJWAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);
}

FActiveGameplayEffectHandle UGA_AbilityBase::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EJWSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EJWSuccessType::Successful : EJWSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}
