// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AnimInstanceBase.h"
#include "Character/CharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UAnimInstanceBase::UAnimInstanceBase()
	:m_IsLive(true)
{
	// Montage의 세팅은 에디터에서 세팅하자. 
	m_MontageList.SetNum(static_cast<uint8>(EMontageList::End));


}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	// 이것 자식들에서 Override해서 세팅해주자. 
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		m_Speed = Pawn->GetVelocity().Size();

		m_Horizontal = Pawn->GetVelocity().X;
		m_Vertical = Pawn->GetVelocity().Y;


		// ACharacter로 캐스팅 해주던걸 더 자식단으로 다운 캐스팅하였다.
		auto Character = Cast<ACharacterBase>(Pawn);
		if (IsValid(Character))
		{
			m_IsFalling = Character->GetMovementComponent()->IsFalling();
			// Die랑 End는 사망으로 처리한다. 
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

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// 공격 상태
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	// 죽음 상태
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// 스킬 상태
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// 피격 상태
	{
		Montage_Pause(m_MontageList[MontageIndex]);
		break;
	}
	default:	// 예외 또는 실패.
		break;
	}
}

const float UAnimInstanceBase::GetPositionMontage(uint8 MontageIndex)
{
	if (m_MontageList[MontageIndex].IsNull() == true)
		return -1.f;

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// 공격 상태
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	// 죽음 상태
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// 스킬 상태
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// 피격 상태
	{
		return Montage_GetPosition(m_MontageList[MontageIndex]);
		break;
	}
	default:	// 예외 또는 실패.
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
	case static_cast<uint8>(EMontageList::AttackMontage):	// 공격 상태
	{
		Montage_SetPosition(m_MontageList[MontageIndex],NewPosition);
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	 // 죽음 상태
	{
		Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// 스킬 상태
	{
		Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// 피격 상태
	{
		Montage_SetPosition(m_MontageList[MontageIndex], NewPosition);
		break;
	}
	default:	// 예외 또는 실패.
		break;
	}
}

void UAnimInstanceBase::SetAutoBlendoutMontage(uint8 MontageIndex, bool bOnOff)
{
	// (임시)로 uint8 형태로 상태를 받고 있다 .
	if (m_MontageList[MontageIndex].IsNull() == true)
		return;

	switch (MontageIndex)
	{
	case static_cast<uint8>(EMontageList::AttackMontage):	// 공격 상태
	{	
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case static_cast<uint8>(EMontageList::DeathMontage):	// 죽음 상태
	{
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case static_cast<uint8>(EMontageList::SkillMontage):	// 스킬 상태
	{
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case static_cast<uint8>(EMontageList::HitMontage):		// 피격 상태
	{
		m_MontageList[MontageIndex]->bEnableAutoBlendOut = bOnOff;
		break;
	}
	default:	// 예외 또는 실패.
		break;
	}
}

const UAnimMontage* UAnimInstanceBase::GetMontageInst(uint8 MontageIndex)
{
	// 없으면 nullptr이 반환.
	return m_MontageList[MontageIndex];
}
