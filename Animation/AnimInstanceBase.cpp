// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstanceBase.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterComponent/MyStatComponent.h"



UAnimInstanceBase::UAnimInstanceBase()
	:m_IsLive(true),m_IsFalling(false),m_IsSprint(false)
{
	// Montage의 세팅은 에디터에서 세팅하자. 
	m_MontageList.SetNum(static_cast<uint8>(EMontageList::End));


}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	// 이것 자식들에서 Override해서 세팅해주자. 
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn) == true)
	{
		m_Speed = Pawn->GetVelocity().Size();

		// (임시) 크기를 -1,1사이 값으로 보간 해두자.
		//m_Horizontal =	FMath::Clamp(Pawn->GetVelocity().X,-1.0,1.0);
		m_Vertical	 =	FMath::Clamp(Pawn->GetVelocity().Y,-1.0,1.0);
		// (임시) 값을 보간해 버리자 ... -1 0 1 로만 만들자.
		if (m_Vertical > 0.f)
			m_Vertical = 1.f;
		else if (m_Vertical < 0.f)
			m_Vertical = -1.f;
		else
			m_Vertical = 0.f;


		// ACharacter로 캐스팅 해주던걸 더 자식단으로 다운 캐스팅하였다.
		auto Character = Cast<ACharacterBase>(Pawn);
		if (IsValid(Character) == true)
		{
			m_IsFalling = Character->GetMovementComponent()->IsFalling();
			// Die랑 End는 사망으로 처리한다. 
			m_IsLive = (Character->GetState() < EBehaviorState::Die);
			// 가속 여부
			m_IsAccelerating = (FMath::Abs(Character->GetCharacterMovement()->GetCurrentAcceleration().Length()) > 0.0);
			// (임시) Horizontal은 캐릭터의 값을 참고하자.
			//m_Vertical = Character->GetVertical();
			m_Horizontal = Character->GetHorizontal();

			m_CharacterState = static_cast<EBehaviorState>(Character->GetState());
			m_CharacterMode = static_cast<ECharacterMode>(Character->GetMode());

			m_AttackSpeedRate = Character->GetStatComponent().GetAttackSpeed();
			// (임시) 무브 재생속도를 임시로 애니메이션 재생속도값을 넣어두었다.
			m_MoveSpeedRate = Character->GetAnimationSpeed();
			
		}
	}
}

const int32 UAnimInstanceBase::GetMontageSectionNum(uint8 MontageIndex)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
	{
		// fail
		return -1;
	}

	return m_MontageList[MontageIndex]->CompositeSections.Num();	
}

void UAnimInstanceBase::PlaySkillMontage(uint8 SkillIndex,float PlaySpeed)
{
	if (m_SkillMontageList[SkillIndex].IsNull() == false)
		Montage_Play(m_SkillMontageList[SkillIndex], PlaySpeed);
}

void UAnimInstanceBase::StopSkillMontage(uint8 SkillIndex, float InBlendOutTime)
{
	if (m_SkillMontageList[SkillIndex].IsNull() == false)
		Montage_Stop(InBlendOutTime, m_SkillMontageList[SkillIndex]);
}

void UAnimInstanceBase::JumpToSection_Skill(uint8 SkillIndex, FName SectionName)
{
	if (m_SkillMontageList[SkillIndex].IsNull() == false)
		Montage_JumpToSection(SectionName, m_SkillMontageList[SkillIndex]);
}
/* Legacy Code
void UAnimInstanceBase::PlaySkillMontage(float PlaySpeed)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)].IsNull() == false)
		Montage_Play(m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)], PlaySpeed);
}

void UAnimInstanceBase::StopSkillMontage(float InBlendOutTime)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)].IsNull() == false)
		Montage_Stop(InBlendOutTime, m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)]);
}

void UAnimInstanceBase::JumpToSection_Skill(FName SectionName)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)].IsNull() == false)
		Montage_JumpToSection(SectionName, m_MontageList[static_cast<uint8>(EMontageList::SkillMontage)]);
}
*/
void UAnimInstanceBase::PlayMontage(uint8 MontageIndex, float PlaySpeed)
{
	if (m_MontageList[MontageIndex].IsNull() == false)
		Montage_Play(m_MontageList[MontageIndex], PlaySpeed);
}

void UAnimInstanceBase::StopMontage(uint8 MontageIndex, float InBlendOutTime)
{
	if (m_MontageList[MontageIndex].IsNull() == false)
		Montage_Stop(InBlendOutTime, m_MontageList[MontageIndex]);
}

void UAnimInstanceBase::JumpToSection(FName SectionName, uint8 MontageIndex)
{
	if (m_MontageList[MontageIndex].IsNull() == false)
	{
		if (MontageIndex == static_cast<uint8>(EMontageList::AttackMontage))
		{
			// 만약에 공격모션은 Index를 받아서 세팅해주는 다른 로직을 가지고있어서 예외 처리해줘야한다.
			// (임시) 0U을 잠시 세팅해주는중. 
			Montage_JumpToSection(GetAttackMontageName(0U), m_MontageList[MontageIndex]);
		}
		else
		{
			Montage_JumpToSection(SectionName, m_MontageList[MontageIndex]);
		}
	}
}

void UAnimInstanceBase::StopAllMontage(float InBlendOutTime)
{
	//Montage_Stop(InBlendOutTime);
	// 원래 기본 BlendOutTime은 0.25f 지만 여기서는 0.f로 설정해주고 있다.
	StopAllMontages(InBlendOutTime);
}

void UAnimInstanceBase::PauseMontage(uint8 MontageIndex)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return;

	Montage_Pause(m_MontageList[MontageIndex]);
}

const float UAnimInstanceBase::GetPositionMontage(uint8 MontageIndex)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return -1.f;

	return Montage_GetPosition(m_MontageList[MontageIndex]);
}

void UAnimInstanceBase::SetPositionMontage(uint8 MontageIndex, float NewPosition)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return;

	Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
}

void UAnimInstanceBase::SetAutoBlendoutMontage(uint8 MontageIndex, bool bOnOff)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return;

	m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
}

const UAnimMontage* UAnimInstanceBase::GetMontageInst(uint8 MontageIndex)
{
	// 없으면 nullptr이 반환.
	return m_MontageList[MontageIndex];
}
