// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstanceBase.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterComponent/MyStatComponent.h"



UAnimInstanceBase::UAnimInstanceBase()
	:m_IsLive(true),m_IsFalling(false),m_IsSprint(false)
{
	// Montage�� ������ �����Ϳ��� ��������. 
	m_MontageList.SetNum(static_cast<uint8>(EMontageList::End));


}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	// �̰� �ڽĵ鿡�� Override�ؼ� ����������. 
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn) == true)
	{
		m_Speed = Pawn->GetVelocity().Size();

		// (�ӽ�) ũ�⸦ -1,1���� ������ ���� �ص���.
		//m_Horizontal =	FMath::Clamp(Pawn->GetVelocity().X,-1.0,1.0);
		m_Vertical	 =	FMath::Clamp(Pawn->GetVelocity().Y,-1.0,1.0);
		// (�ӽ�) ���� ������ ������ ... -1 0 1 �θ� ������.
		if (m_Vertical > 0.f)
			m_Vertical = 1.f;
		else if (m_Vertical < 0.f)
			m_Vertical = -1.f;
		else
			m_Vertical = 0.f;


		// ACharacter�� ĳ���� ���ִ��� �� �ڽĴ����� �ٿ� ĳ�����Ͽ���.
		auto Character = Cast<ACharacterBase>(Pawn);
		if (IsValid(Character) == true)
		{
			m_IsFalling = Character->GetMovementComponent()->IsFalling();
			// Die�� End�� ������� ó���Ѵ�. 
			m_IsLive = (Character->GetState() < EBehaviorState::Die);
			// ���� ����
			m_IsAccelerating = (FMath::Abs(Character->GetCharacterMovement()->GetCurrentAcceleration().Length()) > 0.0);
			// (�ӽ�) Horizontal�� ĳ������ ���� ��������.
			//m_Vertical = Character->GetVertical();
			m_Horizontal = Character->GetHorizontal();

			m_CharacterState = static_cast<EBehaviorState>(Character->GetState());
			m_CharacterMode = static_cast<ECharacterMode>(Character->GetMode());

			m_AttackSpeedRate = Character->GetStatComponent().GetAttackSpeed();
			// (�ӽ�) ���� ����ӵ��� �ӽ÷� �ִϸ��̼� ����ӵ����� �־�ξ���.
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
			// ���࿡ ���ݸ���� Index�� �޾Ƽ� �������ִ� �ٸ� ������ �������־ ���� ó��������Ѵ�.
			// (�ӽ�) 0U�� ��� �������ִ���. 
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
	// ���� �⺻ BlendOutTime�� 0.25f ���� ���⼭�� 0.f�� �������ְ� �ִ�.
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
	// ������ nullptr�� ��ȯ.
	return m_MontageList[MontageIndex];
}
