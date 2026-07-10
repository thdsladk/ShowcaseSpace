// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TargetSystemComponent.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "Physics/JWCollision.h"
#include "Interface/CollisionTeamInterface.h"
#include "Character/CharacterBase.h"
#include "MotionWarpingComponent.h"

// Sets default values for this component's properties
UTargetSystemComponent::UTargetSystemComponent() :
	m_SearchRadius(500.f),
	m_UpdatePeriod(0.1f),
	m_IsTargeting(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTargetSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(m_UpdateTimerHandle,this,&UTargetSystemComponent::UpdateAutoTargetSystem,m_UpdatePeriod,false);
}

void UTargetSystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld()->GetTimerManager().IsTimerActive(m_UpdateTimerHandle) == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(m_UpdateTimerHandle);
	}


}

void UTargetSystemComponent::EnableAutoTargetSystem()
{
	// 타겟 시스템 활성화
	//AutoTargetOn();
	if (m_IsTargeting == false)
	{
		m_IsTargeting = true;
		GetWorld()->GetTimerManager().SetTimer(m_UpdateTimerHandle, this, &UTargetSystemComponent::UpdateAutoTargetSystem, m_UpdatePeriod, true,0.f);
	}
	else
	{
		m_IsTargeting = false;
		Cast<ACharacter>(m_AutoTarget)->GetMesh()->SetRenderCustomDepth(false);
		m_AutoTarget = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(m_UpdateTimerHandle);
	}
}

AActor* UTargetSystemComponent::GetAutoTarget() const
{
	return m_AutoTarget.Get();
}

AActor* UTargetSystemComponent::GetTargetActor() const
{
	return (m_TargetActors.IsEmpty() == true) ? nullptr : m_TargetActors[0].Get();
}	  

TArray<TWeakObjectPtr<AActor>>& UTargetSystemComponent::GetTargetActors()
{
	return m_TargetActors;
}

void UTargetSystemComponent::AddTargetActor(AActor* TargetActor)
{
	m_TargetActors.Add(TargetActor);
}

void UTargetSystemComponent::StartTargeting()
{
	// 타겟 시스템 활성화(초기화)
	m_IsTargeting = true;

}

void UTargetSystemComponent::EndTargeting()
{
	// 타겟 시스템 비활성화 
	CurrentTargetData = FTargetData();

	m_IsTargeting = false;
}

void UTargetSystemComponent::UpdateAutoTargetSystem()
{
	// 여기에서 타겟 시스템을 업데이트할 기능들을 확장
	if (AutoTargetOn() == true)
	{

	}
}

bool UTargetSystemComponent::AutoTargetOn()
{
	// (임시) 하나의 타겟만 생성하는 방식으로 임시로 만든것 

	FHitResult OutHitResult;

	// (임시) 지금은 원점에 Sweep을 발동 시키지만 이것도 확실하게 가장 가까운 타겟을 반환이 보장되는지 모르겠다.
	//		  따라서 직접 Overlap으로 여러 타겟을 지정하고 가장 가까운 순서대로 정렬하는 방식이 필요할수도 있을꺼 같다. 
	bool bHit = UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(
		GetOwner(),
		0.f,
		FCollisionShape::MakeSphere(m_SearchRadius),
		CChannel::JWPlayerAttack,
		OutHitResult,
		TEXT("AutoTargetOn"));

	if (bHit == true)
	{
		if (m_AutoTarget == nullptr)
		{
			
			// 타겟 변경.
			m_AutoTarget = (OutHitResult.GetActor());
			
			// 새로운 타겟에 커스텀 뎁스 설정
			USkeletalMeshComponent* SK = Cast<ACharacter>(m_AutoTarget)->GetMesh();
			if (SK != nullptr)
			{
				SK->SetRenderCustomDepth(true);
				SK->SetRenderInDepthPass(true);
				SK->SetCustomDepthStencilValue(1);
			}
		}
		else if (m_AutoTarget != OutHitResult.GetActor())
		{
			// 타겟이 변경될 경우에 이전 타겟의 커스텀 뎁스는 해제되고 새로운 타겟은 설정이 된다.
			Cast<ACharacter>(m_AutoTarget)->GetMesh()->SetRenderCustomDepth(false);

			m_AutoTarget = (OutHitResult.GetActor());

			// 새로운 타겟에 커스텀 뎁스 설정
			USkeletalMeshComponent* SK = Cast<ACharacter>(m_AutoTarget)->GetMesh();
			if (SK != nullptr)
			{
				SK->SetRenderCustomDepth(true);
				SK->SetRenderInDepthPass(true);
				SK->SetCustomDepthStencilValue(1);
			}
		}
		else
		{
			// 충돌이 없는 걸로 처리해서 실행을 안하도록 한다. 
			return false;
		}
	}
	else
	{
		// 타겟이 없을 경우 

	}

	return bHit;
}


