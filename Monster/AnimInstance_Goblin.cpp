// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Goblin.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Character/CharacterBase.h"

UAnimInstance_Goblin::UAnimInstance_Goblin() 
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Game/TopDown/Animation/SK_Goblin_Skeleton_Montage_Attack.SK_Goblin_Skeleton_Montage_Attack"));
	if (AM.Succeeded())
	{
		m_MontageList[static_cast<uint8>(EMontageList::AttackMontage)] = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DAM(TEXT("/Game/TopDown/Animation/SK_Goblin_Skeleton_Montage_Death.SK_Goblin_Skeleton_Montage_Death"));
	if (DAM.Succeeded())
	{
		m_MontageList[static_cast<uint8>(EMontageList::DeathMontage)] = DAM.Object;
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
		auto NPC = Cast<ACharacterBase>(Pawn);
		if (NPC)
		{
			m_IsFalling = NPC->GetMovementComponent()->IsFalling();
			// Die�� End�� ������� ó���Ѵ�. 
			m_IsLive = (NPC->GetState() < static_cast<uint8>(EBehaviorState::Die));

			//m_Vertical = NPC->GetVertical();
			//m_Horizontal = NPC->GetHorizontal();

			m_CharacterState = static_cast<EBehaviorState>(NPC->GetState());
		}
	}
}

