// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Task/AT_PlayDodgeAndWait.h"
#include "GameFramework/Character.h"
//#include "Character/CharacterBase.h"
#include "Interface/MoveInterface.h"
#include "Interface/PlayerControllerInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemGlobals.h"

#include "Interface/AnimNotifyInterface.h"


UAT_PlayDodgeAndWait::UAT_PlayDodgeAndWait()
{
	//bTickingTask = true;
	bTickingTask = false;
}

UAT_PlayDodgeAndWait* UAT_PlayDodgeAndWait::CreateDodgeAndWaitProxy(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds)
{

	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAT_PlayDodgeAndWait* MyObj = NewAbilityTask<UAT_PlayDodgeAndWait>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	
	return MyObj;
}

void UAT_PlayDodgeAndWait::Activate()
{
	Super::Activate();

	//ACharacter* Character = CastChecked<ACharacter>(Ability->GetAvatarActorFromActorInfo());
	//IAnimNotifyInterface* ANI = CastChecked<IAnimNotifyInterface>(Character->GetMesh()->GetAnimInstance());
	//FOnEndMove Delegate;
	//Delegate.AddUObject(this, &UAT_PlayDodgeAndWait::TurnOffTick);
	//ANI->BindEndMove(Delegate);
	
}

void UAT_PlayDodgeAndWait::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
	
}

void UAT_PlayDodgeAndWait::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	if (bTickingTask == true)
	{
		//ACharacter* Character = CastChecked<ACharacter>(Ability->GetAvatarActorFromActorInfo());
		//Character->AddMovementInput(Character->GetActorForwardVector(), 1.0f);
		//
		//IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Character->GetController());
		//CastChecked<IMoveInterface>(Character)->RotateToInterp(PCI->GetCachedInputVector(), 1440.f);
	}
}