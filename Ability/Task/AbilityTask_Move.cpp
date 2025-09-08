// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Task/AbilityTask_Move.h"
#include "MyAbilityInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UAbilityTask_Move::Start(UMyAbilityInstance* Owner)
{
    Super::Start(Owner);

	m_pOwner = Owner;
    if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveForwardTask Start failed: Invalid owner"));
        Cancel();
        return;
    }

    // 캐릭터 얻기
    ACharacter* Character = CastChecked<ACharacter>(m_pOwner->GetAbilityComponent()->GetOwner());
    m_StartLocation = Character->GetActorLocation();
    m_Duration = m_pOwner->GetAbilityData()->m_pMontage->GetPlayLength();
    m_TargetLocation = m_StartLocation + (Character->GetActorForwardVector().GetSafeNormal() * m_Distance);


    switch (m_MoveType)
    {
    case EMoveType::None:
    {
        Cancel();
        break;
    }
    case EMoveType::Walk:
    {
		 break;
    }
    case EMoveType::Sprint:
    {

        break;
    }
    case EMoveType::CrouchWalk:
    {

        break;
    }
    case EMoveType::Jump:
    {
        // 타겟 위치 세팅 
        FVector Force = (Character->GetActorForwardVector().GetSafeNormal() * m_Distance);
        Force.Z += m_MaxHeight;

        Character->LaunchCharacter(Force, true, true);

        break;
    }
    case EMoveType::Fly:
    {

        break;
    }
    case EMoveType::Swim:
    {

        break;
    }
    default:
        break;
    }

}

void UAbilityTask_Move::Tick(float DeltaTime)
{
    if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
    {
        Cancel();
        return;
    }

    m_ElapsedTime += DeltaTime;
    float Time = FMath::Clamp(m_ElapsedTime / m_Duration, 0.f, 1.f);

    if(m_MoveType != EMoveType::Jump)
    {
        // 캐릭터 얻기
        ACharacter* Character = CastChecked<ACharacter>(m_pOwner->GetAbilityComponent()->GetOwner());
        FVector NewLocation = FMath::Lerp(m_StartLocation, m_TargetLocation, Time);
        Character->SetActorLocation(NewLocation);
	}


    if (Time >= 1.f)
    {
        Cancel(); // 종료
    }

}

void UAbilityTask_Move::Cancel()
{
    if (m_pOwner != nullptr)
    { 
        m_pOwner = nullptr;
        m_OnCompleted.Broadcast();
    }
    else
    {
        return;
    }

}

bool UAbilityTask_Move::WantsTick() const
{
    if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
        return false;

    // 예: 타겟 위치가 현재 위치랑 같으면 
    if (m_TargetLocation.Equals(m_pOwner->GetAbilityComponent()->GetOwner()->GetActorLocation()) == true)
        return false;

    // 예: 특정 태그를 가지고 있을 때만 실행
    //return m_pOwner->GetAbilityComponent()->CheckOwnedTags(m_pOwner->GetAbilityData()->m_Exclusivity.ApplyWhileActive);
    return true;

}
