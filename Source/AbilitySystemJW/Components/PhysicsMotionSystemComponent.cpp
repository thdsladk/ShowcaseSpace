// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PhysicsMotionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPhysicsMotionSystemComponent::UPhysicsMotionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPhysicsMotionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPhysicsMotionSystemComponent::Tick_Force()
{
	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());

	// 물리 시뮬레이션을 꺼두면 작동 안한다. [ 물리 시뮬레이션을 키면 캡슐이 충돌을 안해서 지형밑으로 떨어지는 문제.. ] 
	Owner->GetCharacterMovement()->AddForce(m_Force);
}

void UPhysicsMotionSystemComponent::PlayKnockBack(const FVector& Direction, float Scale)
{
	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	m_Direction = Direction;
	m_Scale = Scale;


	if (Direction.IsNearlyZero() == true)
	{
		//CastChecked<ACharacter>(GetOwner())->GetCharacterMovement()->AddImpulse(( - (GetOwner()->GetActorForwardVector()) * m_Scale));
		Owner->LaunchCharacter((-( Owner->GetActorForwardVector()) * m_Scale ),true,false);
	}
	else
	{
		//CastChecked<ACharacter>(GetOwner())->GetCharacterMovement()->AddImpulse((m_Direction * m_Scale));
		Owner->LaunchCharacter((Direction * m_Scale), true, false);
	}
}

void UPhysicsMotionSystemComponent::PlayKnockBack(const FVector& Force)
{
	ACharacter* Owner = CastChecked<ACharacter>(GetOwner());
	m_Force = Force;
	if (Force.IsNearlyZero() == true)
	{
		// 이 경우는 힘이 없는 넉백이다. 무효
	}
	else
	{
		//Owner->GetCharacterMovement()->AddImpulse(Force);
		Owner->LaunchCharacter(Force, true, true);
	}
}

void UPhysicsMotionSystemComponent::StartAddForce(const FVector& Force)
{
	m_Force = Force;
	GetWorld()->GetTimerManager().SetTimer(m_ForceTimer, this, &UPhysicsMotionSystemComponent::Tick_Force, 0.001f, true);
}

void UPhysicsMotionSystemComponent::EndAddForce()
{
	m_Force = FVector::ZeroVector;
	GetWorld()->GetTimerManager().ClearTimer(m_ForceTimer);
}




