// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Dodge.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "Character/CharacterBase.h"
#include "Interface/PlayerControllerInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GA/Task/AT_PlayDodgeAndWait.h" 
#include "Animation/AnimInstanceBase.h"
#include "MotionWarpingComponent.h"

#include "AbilitySystemJW.h"

UGA_Dodge::UGA_Dodge() 
	: m_Cost(0.f)
	, m_Distance(500.0)
	, m_RatioSpeed(3.f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC != nullptr)
	{
		const UCharacterAttributeSet* Attribute = ASC->GetSet<UCharacterAttributeSet>();
		if (Attribute != nullptr)
		{
			if (Attribute->GetStamina() >= m_Cost)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Super::CommitAbility(Handle, ActorInfo, ActivationInfo);

	ACharacterBase* Character = CastChecked<ACharacterBase>(ActorInfo->AvatarActor.Get());
	CastChecked<IPlayerControllerInterface>(Character->GetController())->SetControlMode(uint8(EPlayerControlMode::NoGameplayInput));

	ComputeDodgeDirectionAndDistance();
	
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
	UAT_PlayDodgeAndWait* PlayDodgeTask = UAT_PlayDodgeAndWait::CreateDodgeAndWaitProxy(this, TEXT("PlayDodge"), AnimInstance->GetCurrentDodgeMontage(), 1.0f, GetDodgeSectionName());
	PlayDodgeTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnCompleteCallback);
	PlayDodgeTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnInterruptedCallback);
	PlayDodgeTask->ReadyForActivation();



}

void UGA_Dodge::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	APawn* Character = CastChecked<APawn>(ActorInfo->AvatarActor.Get());
	IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(Character->GetController());
	if (PCI != nullptr)
	{
		PCI->SetControlMode(uint8(EPlayerControlMode::NormalMode));

	}


}

void UGA_Dodge::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

void UGA_Dodge::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

FName UGA_Dodge::GetDodgeSectionName()
{
	return FName(TEXT("Dodge"));
}

void UGA_Dodge::ComputeDodgeDirectionAndDistance()
{
	// Compute Direction
	ACharacterBase* Character = CastChecked<ACharacterBase>(GetAvatarActorFromActorInfo());
	FVector2D Dir2D = CastChecked<IPlayerControllerInterface>(Character->GetController())->GetCachedInputVector().GetSafeNormal();
	FVector Dir3D = FVector(Dir2D, 0.f);
	//FVector Dir2D = Character->GetLastMovementInputVector().GetSafeNormal();
	
	// Compute Distance
	FVector Start = Character->GetActorLocation();
	FVector End = Start + (Dir3D * m_Distance);

	FCollisionQueryParams Params(FName("DodgeTrace"),false,Character);

	FHitResult Hit;
	bool bHit = Character->GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	FVector Destination;
	if (bHit ==true)
	{
		// 충돌한 지점 바로 앞까지 이동
		Destination = Hit.Location - Dir3D * 10.f; // 살짝 여유를 둠
	}
	else
	{
		// 충돌 없으면 원래 계산한 End 사용
		Destination = End;
	}


	Character->GetMotionWarpingComp().AddOrUpdateWarpTargetFromLocationAndRotation(FName("DodgeDirection"), FVector(), Dir3D.Rotation());
	Character->GetMotionWarpingComp().AddOrUpdateWarpTargetFromLocationAndRotation(FName("DodgeDistance"), Destination, Dir3D.Rotation());

}
