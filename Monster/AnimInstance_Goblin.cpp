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
		// 이동속도를 300으로 맞춰두었다. BS_BattleMove 최대치 600으로 세팅해둠.
		// 그로인해서 방패들 살짝 들고 이동하는 상태가 된다. 
		m_Horizontal = Pawn->GetVelocity().X;
		m_Vertical = Pawn->GetVelocity().Y;


		// ACharacter로 캐스팅 해주던걸 더 자식단으로 다운 캐스팅하였다.
		auto NPC = Cast<ACharacterBase>(Pawn);
		if (NPC)
		{
			m_IsFalling = NPC->GetMovementComponent()->IsFalling();
			// Die랑 End는 사망으로 처리한다. 
			m_IsLive = (NPC->GetState() < static_cast<uint8>(EBehaviorState::Die));

			//m_Vertical = NPC->GetVertical();
			//m_Horizontal = NPC->GetHorizontal();

			m_CharacterState = static_cast<EBehaviorState>(NPC->GetState());
		}
	}
}

