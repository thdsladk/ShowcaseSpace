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

    // ĳ���� ���
    ACharacter* Character = CastChecked<ACharacter>(m_pOwner->GetAbilityComponent()->GetOwner());

    m_StartLocation = Character->GetActorLocation();

    m_Duration = m_pOwner->GetAbilityData()->m_pMontage->GetPlayLength();

    


    // Ÿ�� ��ġ ���� 
    m_TargetLocation = m_StartLocation + (Character->GetActorForwardVector().GetSafeNormal() * m_Distance);

    // �߷� ���� 
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


    // ���� ����
    FVector Horizontal = FMath::Lerp(m_StartLocation, m_TargetLocation, Time);

    // ������ ���� ��� (������ Duration/2 ����)
    float Height = m_MaxHeight - (4.f * m_MaxHeight * FMath::Square(Time - 0.5f));

    Horizontal.Z += Height;

    Character->SetActorLocation(Horizontal, true);

    if (Time >= 1.f)
    {
        Cancel(); // ����
    }


}

void UAbilityTask_Move::Cancel()
{

    // ������ �̵� ���� �� �߷� ����
    ACharacter* Character = CastChecked<ACharacter>(m_pOwner->GetAbilityComponent()->GetOwner());
    if (Character->GetCharacterMovement() != nullptr)
    {
        Character->GetCharacterMovement()->SetMovementMode(m_OriginalMode);

        // (�ӽ�) ������ �ν��Ͻ��� ���� �ּҸ� nullptr�� ����� ƽ�� ����.
        m_pOwner = nullptr;
    }
}

bool UAbilityTask_Move::WantsTick() const
{
    if (m_pOwner == nullptr || m_pOwner->GetAbilityComponent() == nullptr)
        return false;

    // ��: Ÿ�� ��ġ�� ���� ��ġ�� ������ 
    if (m_TargetLocation.Equals(m_pOwner->GetAbilityComponent()->GetOwner()->GetActorLocation()) == true)
        return false;

    // ��: Ư�� �±׸� ������ ���� ���� ����
    //return m_pOwner->GetAbilityComponent()->CheckOwnedTags(m_pOwner->GetAbilityData()->m_Exclusivity.ApplyWhileActive);
    return true;

}
