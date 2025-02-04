// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyTest_TopDownCharacter.h"

UMyAnimInstance::UMyAnimInstance()
	:IsLive(true)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Script/Engine.AnimMontage'/Game/TopDown/Animation/Grux_Skeleton_Montage.Grux_Skeleton_Montage'"));
	if (AM.Succeeded())
	{
		m_AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DAM(TEXT("/Script/Engine.AnimMontage'/Game/TopDown/Animation/Grux_Skeleton_Montage_Death_B.Grux_Skeleton_Montage_Death_B'"));
	if (DAM.Succeeded())
	{
		m_DeathMontage = DAM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SM(TEXT("/Script/Engine.AnimMontage'/Game/TopDown/Animation/Grux_Skeleton_Montage_Skill.Grux_Skeleton_Montage_Skill'"));
	if (SM.Succeeded())
	{
		SkillMontage = SM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HM(TEXT("/Script/Engine.AnimMontage'/Game/TopDown/Animation/Grux_Skeleton_Montage_Hit.Grux_Skeleton_Montage_Hit'"));
	if (HM.Succeeded())
	{
		HitMontage = HM.Object;
	}
	
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		m_Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<AMyTest_TopDownCharacter>(Pawn);
		if (Character)
		{
			IsFalling = Character->GetMovementComponent()->IsFalling();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), IsFalling? TEXT("True") : TEXT("False"))
			m_Vertical = Character->GetVertical();
			m_Horizontal = Character->GetHorizontal();
			m_State = Character->GetState();
		}
	}

}

void UMyAnimInstance::PlayAttackMontage()
{
	Montage_Play(m_AttackMontage,1.f);
}

void UMyAnimInstance::JumpToSection_Attack(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, m_AttackMontage);
}

void UMyAnimInstance::PlayDeathMontage()
{
	Montage_Play(m_DeathMontage, 1.f);
}

void UMyAnimInstance::JumpToSection_Death(FName SectionName)
{
	Montage_JumpToSection(SectionName, m_DeathMontage);
}

void UMyAnimInstance::PlaySkillMontage()
{
	Montage_Play(SkillMontage, 1.f);
}

void UMyAnimInstance::JumpToSection_Skill(FName SectionName)
{
	Montage_JumpToSection(SectionName, SkillMontage);
}

void UMyAnimInstance::PlayHitMontage()
{
	Montage_Play(HitMontage, 1.f);
}

void UMyAnimInstance::JumpToSection_Hit(FName SectionName)
{
	Montage_JumpToSection(SectionName, HitMontage);
}

FName UMyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UMyAnimInstance::AnimNotify_AttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast();
}

void UMyAnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}

void UMyAnimInstance::AnimNotify_DeathPoint()
{
	OnDeathPoint.Broadcast();
}

void UMyAnimInstance::AnimNotify_SkillPoint()
{
	OnSkillPoint.Broadcast();
}

void UMyAnimInstance::AnimNotify_SkillEnd()
{
	OnSkillEnd.Broadcast();
}

void UMyAnimInstance::AnimNotify_HitEnd()
{
	OnHitEnd.Broadcast();
}

void UMyAnimInstance::AnimTrail_Trail()
{

}
