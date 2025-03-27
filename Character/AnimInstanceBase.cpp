// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimInstanceBase.h"
#include "Character/CharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UAnimInstanceBase::UAnimInstanceBase()
	:m_IsLive(true)
{
	// Montage�� ������ �����Ϳ��� ��������. 
	m_MontageList.SetNum(static_cast<uint8>(EMontageList::End));


}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	// �̰� �ڽĵ鿡�� Override�ؼ� ����������. 
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		m_Speed = Pawn->GetVelocity().Size();

		m_Horizontal = Pawn->GetVelocity().X;
		m_Vertical = Pawn->GetVelocity().Y;


		// ACharacter�� ĳ���� ���ִ��� �� �ڽĴ����� �ٿ� ĳ�����Ͽ���.
		auto Character = Cast<ACharacterBase>(Pawn);
		if (IsValid(Character))
		{
			m_IsFalling = Character->GetMovementComponent()->IsFalling();
			// Die�� End�� ������� ó���Ѵ�. 
			m_IsLive = (Character->GetState() < static_cast<uint8>(EBehaviorState::Die));

			//m_Vertical = Character->GetVertical();
			//m_Horizontal = Character->GetHorizontal();

			m_CharacterState = static_cast<EBehaviorState>(Character->GetState());
		}
	}
}

void UAnimInstanceBase::PlayAttackMontage(float PlaySpeed)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)].IsNull() == false)
		Montage_Play(m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)], PlaySpeed);
}

void UAnimInstanceBase::StopAttackMontage(float InBlendOutTime)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)].IsNull() == false)
		Montage_Stop(InBlendOutTime, m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)]);
}

void UAnimInstanceBase::JumpToSection_Attack(int32 SectionIndex)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)].IsNull() == false)
		Montage_JumpToSection(GetAttackMontageName(SectionIndex), m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)]);
}

void UAnimInstanceBase::PlayDeathMontage(float PlaySpeed)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)].IsNull() == false)
		Montage_Play(m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)], PlaySpeed);
}

void UAnimInstanceBase::StopDeathMontage(float InBlendOutTime)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)].IsNull() == false)
		Montage_Stop(InBlendOutTime, m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)]);
}

void UAnimInstanceBase::JumpToSection_Death(FName SectionName)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)].IsNull() == false)
		Montage_JumpToSection(SectionName, m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)]);
}

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

void UAnimInstanceBase::PlayHitMontage(float PlaySpeed)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::HitMontage)].IsNull() == false)
		Montage_Play(m_MontageList[static_cast<uint8>(EMontageList::HitMontage)], PlaySpeed);
}

void UAnimInstanceBase::StopHitMontage(float InBlendOutTime)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::HitMontage)].IsNull() == false)
		Montage_Stop(InBlendOutTime, m_MontageList[static_cast<uint8>(EMontageList::HitMontage)]);
}

void UAnimInstanceBase::JumpToSection_Hit(FName SectionName)
{
	if (m_MontageList[static_cast<uint8>(EMontageList::HitMontage)].IsNull() == false)
		Montage_JumpToSection(SectionName, m_MontageList[static_cast<uint8>(EMontageList::HitMontage)]);
}

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

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// ���� ����
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	// ���� ����
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// ��ų ����
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// �ǰ� ����
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	default:	// ���� �Ǵ� ����.
		break;
	}
}

const float UAnimInstanceBase::GetPositionMontage(uint8 MontageIndex)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return -1.f;

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// ���� ����
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	// ���� ����
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// ��ų ����
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// �ǰ� ����
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	default:	// ���� �Ǵ� ����.
		return -1.f;
		break;
	}
}

void UAnimInstanceBase::SetPositionMontage(uint8 MontageIndex, float NewPosition)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return;

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// ���� ����
	{
		Montage_SetPosition(m_MontageList[MontageIndex],NewPosition);
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	 // ���� ����
	{
		Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// ��ų ����
	{
		Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// �ǰ� ����
	{
		Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
		break;
	}
	default:	// ���� �Ǵ� ����.
		break;
	}
}

void UAnimInstanceBase::SetAutoBlendoutMontage(uint8 MontageIndex, bool bOnOff)
{
	// (�ӽ�)�� uint8 ���·� ���¸� �ް� �ִ� .
	if (m_MontageList[MontageIndex].IsNull() == true)
		return;

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// ���� ����
	{	
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	// ���� ����
	{
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// ��ų ����
	{
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// �ǰ� ����
	{
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	default:	// ���� �Ǵ� ����.
		break;
	}
}

const UAnimMontage* UAnimInstanceBase::GetMontageInst(uint8 MontageIndex)
{
	// ������ nullptr�� ��ȯ.
	return m_MontageList[MontageIndex];
}
