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

    


    // 타겟 위치 세팅 
    m_TargetLocation = m_StartLocation + (Character->GetActorForwardVector().GetSafeNormal() * m_Distance);

    // 중력 제거 
    UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
    if (MoveComp != nullptr)
    {
        m_OriginalMode = MoveComp->MovementMode;
        MoveComp->SetMovementMode(MOVE_Flying);
    }



}

void UAbilityTask_Move::Tick(float DeltaTime)
{
    if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
    {
        Cancel();
        return;
    }
    
    ACharacter* Character = CastChecked<ACharacter>(m_pOwner->GetAbilityComponent()->GetOwner());

    m_ElapsedTime += DeltaTime;
    float Time = FMath::Clamp(m_ElapsedTime / m_Duration, 0.f, 1.f);


    // 수평 보간
    FVector Horizontal = FMath::Lerp(m_StartLocation, m_TargetLocation, Time);

    // 포물선 높이 계산 (정점은 Duration/2 시점)
    float Height = m_MaxHeight - (4.f * m_MaxHeight * FMath::Square(Time - 0.5f));

    Horizontal.Z += Height;

    Character->SetActorLocation(Horizontal, true);

    if (Time >= 1.f)
    {
        Cancel(); // 종료
    }


}

void UAbilityTask_Move::Cancel()
{

    // 포물선 이동 종료 → 중력 복구
    ACharacter* Character = CastChecked<ACharacter>(m_pOwner->GetAbilityComponent()->GetOwner());
    if (Character->GetCharacterMovement() != nullptr)
    {
        Character->GetCharacterMovement()->SetMovementMode(m_OriginalMode);

        // (임시) 끝나면 인스턴스에 대한 주소를 nullptr로 만들면 틱이 종료.
        m_pOwner = nullptr;
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
