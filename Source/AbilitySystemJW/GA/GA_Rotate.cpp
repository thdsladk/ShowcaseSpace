// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Rotate.h"
#include "Tag/JWGameplayTag.h"
#include "GameFramework/RotatingMovementComponent.h"


UGA_Rotate::UGA_Rotate()
{
	AbilityTags.AddTag(JWTAG_ACTOR_ROTATE);
	ActivationOwnedTags.AddTag(JWTAG_ACTOR_ISROTATING);
}

void UGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if(AvatarActor != nullptr)
	{
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));
		if (RotatingMovement != nullptr)
		{
			RotatingMovement->Activate(true);
		}
	}
}

void UGA_Rotate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor != nullptr)
	{
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));
		if (RotatingMovement != nullptr)
		{
			RotatingMovement->Deactivate();
		}
	}
}
