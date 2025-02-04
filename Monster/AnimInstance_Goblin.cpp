// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Goblin.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Monster_Goblin.h"

UAnimInstance_Goblin::UAnimInstance_Goblin() 
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Script/Engine.AnimMontage'/Game/TopDown/Animation/SK_Goblin_Skeleton_Montage_Attack.SK_Goblin_Skeleton_Montage_Attack'"));
	if (AM.Succeeded())
	{
		m_AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DAM(TEXT("/Script/Engine.AnimMontage'/Game/TopDown/Animation/SK_Goblin_Skeleton_Montage_Death.SK_Goblin_Skeleton_Montage_Death'"));
	if (DAM.Succeeded())
	{
		m_DeathMontage = DAM.Object;
	}

}

void UAnimInstance_Goblin::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		m_Speed = Pawn->GetVelocity().Size();
		// �̵��ӵ��� 300���� ����ξ���. BS_BattleMove �ִ�ġ 600���� �����ص�.
		// �׷����ؼ� ���е� ��¦ ��� �̵��ϴ� ���°� �ȴ�. 
		m_Horizontal = Pawn->GetVelocity().X;
		m_Vertical = Pawn->GetVelocity().Y;


		// ACharacter�� ĳ���� ���ִ��� �� �ڽĴ����� �ٿ� ĳ�����Ͽ���.
		auto Monster = Cast<AMonster_Goblin>(Pawn);
		if (Monster)
		{
			m_IsFalling = Monster->GetMovementComponent()->IsFalling();
			m_IsLive = (Monster->GetState() != AMonster_Goblin::EState::Die);

			//m_Vertical = Monster->GetVertical();
			//m_Horizontal = Monster->GetHorizontal();

			m_State = Monster->GetState();
		}
	}
}

void UAnimInstance_Goblin::PlayAttackMontage()
{
	Montage_Play(m_AttackMontage, 1.f);
}

void UAnimInstance_Goblin::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, m_AttackMontage);
}

void UAnimInstance_Goblin::JumpToSection_Death(FString str)
{
	FName Name = GetDeathMontageName(str);
	Montage_JumpToSection(Name, m_DeathMontage);
}

void UAnimInstance_Goblin::PlayDeathMontage()
{
	Montage_Play(m_DeathMontage, 1.f);
}

void UAnimInstance_Goblin::PlayDeathMontage(float fTime)
{
	Montage_Play(m_DeathMontage, fTime);
}

void UAnimInstance_Goblin::StopMontage(int8 MontageIndex)
{
	// �ӽ÷� int32 ���·� ���¸� �ް� �ִ� .
	switch (MontageIndex)
	{
	case 0:	// ���� ����(�ӽ�)
	{
		Montage_Stop(0.f, m_AttackMontage);
		break;
	}
	case 1: // ���� ���� (�ӽ�)
	{
		Montage_Stop(0.f, m_DeathMontage);
		break;
	}
	default:
		break;
	}

	
}

void UAnimInstance_Goblin::PauseMontage(int8 MontageIndex)
{
	// �ӽ÷� int32 ���·� ���¸� �ް� �ִ� .
	switch (MontageIndex)
	{
	case 0:	// ���� ����(�ӽ�)
	{
		Montage_Pause(m_AttackMontage);
		break;
	}
	case 1: // ���� ���� (�ӽ�)
	{
		Montage_Pause(m_DeathMontage);
		break;
	}
	default:
		break;
	}
}

const float UAnimInstance_Goblin::GetPositionMontage(int8 MontageIndex)
{
	// �ӽ÷� int32 ���·� ���¸� �ް� �ִ� .
	switch (MontageIndex)
	{
	case 0:	// ���� ����(�ӽ�)
	{
		return Montage_GetPosition(m_AttackMontage);
		break;
	}
	case 1: // ���� ���� (�ӽ�)
	{
		return Montage_GetPosition(m_DeathMontage);
		break;
	}
	default:
		return 0.f;
		break;
	}
}

void UAnimInstance_Goblin::SetPositionMontage(int8 MontageIndex, float fTime)
{
	// �ӽ÷� int32 ���·� ���¸� �ް� �ִ� .
	switch (MontageIndex)
	{
	case 0:	// ���� ����(�ӽ�)
	{
		Montage_SetPosition(m_AttackMontage,fTime);
		break;
	}
	case 1: // ���� ���� (�ӽ�)
	{
		Montage_SetPosition(m_DeathMontage, fTime);
		break;
	}
	default:
		break;
	}
}

void UAnimInstance_Goblin::SetAutoBlendoutMontage(int8 MontageIndex, bool bOnOff)
{
	// �ӽ÷� int32 ���·� ���¸� �ް� �ִ� .
	switch (MontageIndex)
	{
	case 0:	// ���� ����(�ӽ�)
	{
		m_AttackMontage->bEnableAutoBlendOut = bOnOff;
		break;
	}
	case 1: // ���� ���� (�ӽ�)
	{
		m_DeathMontage->bEnableAutoBlendOut = bOnOff;
		break;
	}
	default:	// ���� �Ǵ� ����.
		break;
	}
}

FName UAnimInstance_Goblin::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

FName UAnimInstance_Goblin::GetDeathMontageName(FString MontageName)
{
	return FName(*MontageName);
}

const UAnimMontage* UAnimInstance_Goblin::GetMontageInst(int8 MontageIndex)
{
	// �ӽ÷� int32 ���·� ���¸� �ް� �ִ� .
	switch (MontageIndex)
	{
	case 0:	// ���� ����(�ӽ�)
	{
		return m_AttackMontage;
		break;
	}
	case 1: // ���� ���� (�ӽ�)
	{
		return m_DeathMontage;
		break;
	}
	default:
		return nullptr;
		break;
	}
}

void UAnimInstance_Goblin::AnimNotify_AttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit_Goblin"));
	OnAttackHit.Broadcast();
}

void UAnimInstance_Goblin::AnimNotify_AttackEnd()
{
	OnAttackEnd.Broadcast();
}

void UAnimInstance_Goblin::AnimNotify_DeathPoint()
{
	OnDeathPoint.Broadcast();
}
