// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Character/CharacterBase.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Game/TopDown/Animation/Grux_Skeleton_Montage.Grux_Skeleton_Montage"));
	if (AM.Succeeded())
	{
		m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)] = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DAM(TEXT("/Game/TopDown/Animation/Grux_Skeleton_Montage_Death_B.Grux_Skeleton_Montage_Death_B"));
	if (DAM.Succeeded())
	{
		m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)] = DAM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SM(TEXT("/Game/TopDown/Animation/Grux_Skeleton_Montage_Skill.Grux_Skeleton_Montage_Skill"));
	if (SM.Succeeded())
	{
		m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)] = SM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HM(TEXT("/Game/TopDown/Animation/Grux_Skeleton_Montage_Hit.Grux_Skeleton_Montage_Hit"));
	if (HM.Succeeded())
	{
		m_MontageList[static_cast<uint8>(EMontageList::HitMontage)] = HM.Object;
	}
	
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		m_Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacterBase>(Pawn);
		if (Character)
		{
			m_IsFalling = Character->GetMovementComponent()->IsFalling();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), IsFalling? TEXT("True") : TEXT("False"))
			m_Vertical = Character->GetVertical();
			m_Horizontal = Character->GetHorizontal();
			m_CharacterState = static_cast<EBehaviorState>(Character->GetState());
		}
	}

}