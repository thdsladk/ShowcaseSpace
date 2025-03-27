// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_FinalHit.h"
#include "MyAnimInstance.h"
#include "MyStatComponent.h"
#include "Character/CharacterBase.h"

#include "GhostTrail.h"

ASkill_FinalHit::ASkill_FinalHit()
{

	// GhostTrail
	static ConstructorHelpers::FClassFinder<AActor> GhostTrail(TEXT("/Script/MyTest_TopDown.GhostTrail"));
	if (GhostTrail.Succeeded())
	{
		m_pGhostTrail = GhostTrail.Class;
	}

}

void ASkill_FinalHit::UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;

	// AnimInstance Delegate   // 구독하기.
	m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_FinalHit::OnSkillMontageEnded);
	m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_FinalHit::OnSkillMontagePoint);



	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("LevelStart"));

	// 스킬 실행 
	SearchActor_Sphere(1.f, 1200.f, ECollisionChannel::ECC_GameTraceChannel2);

	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_FinalHit, this, &ASkill_FinalHit::PlaySkill, 1.0f, true);
	m_bGhostTrail = true;


	// OnSkillTick 
	int32 Rate = 0.1f;
	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_Tick, this, &ASkill_FinalHit::SkillTick, Rate, true);

}

void ASkill_FinalHit::StopSkill()
{
	m_bGhostTrail = false;
}

void ASkill_FinalHit::PlaySkill()
{
	// 한번의 공격 루틴 

// 리스트 확인
	if (m_TargetList.IsEmpty())
	{
		m_pOwner->EnableInput(Cast<APlayerController>(m_pOwner->GetController()));
		CastChecked<ACharacterBase>(m_pOwner)->SetState(static_cast<uint8>(EBehaviorState::Battle));
		m_bGhostTrail = false;
		m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_FinalHit);
		return;
	}

	AActor* pTarget = m_TargetList.Last();
	if (pTarget == nullptr)
	{
		return;
	}

	// 이동 

	m_pOwner->SetActorLocation(pTarget->GetActorLocation() + (pTarget->GetActorForwardVector() * 50.f));

	FVector LookVector = -(pTarget->GetActorForwardVector());
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	m_pOwner->SetActorRotation(TargetRot);

	// 공격 

	CastChecked<ACharacterBase>(m_pOwner)->Attack();

	// 대기

	m_TargetList.Remove(pTarget);

}

void ASkill_FinalHit::CheckSkill()
{
}

void ASkill_FinalHit::OnEffect()
{
}

void ASkill_FinalHit::OnSkillMontageEnded()
{
	// OffSkillTick 
	m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Tick);

}

void ASkill_FinalHit::OnSkillMontagePoint()
{
}

void ASkill_FinalHit::SkillTick()
{
	if (m_bGhostTrail)
	{
		Effect_GhostTrail();
	}
}

void ASkill_FinalHit::Effect_GhostTrail()
{
	FVector Location = m_pOwner->GetActorLocation();

	AGhostTrail* GhostTrail = Cast<AGhostTrail>(m_pOwner->GetWorld()->SpawnActor(m_pGhostTrail, &Location));
	// (임시) 이것도 추상화 하자 ... 
	GhostTrail->SetupGhostTrail(CastChecked<ACharacter>(m_pOwner));
	GhostTrail->BeginGhostTrailEffect();
}

bool ASkill_FinalHit::SearchActor_Sphere(float Range, float Radius, ECollisionChannel Channel)
{
	if (m_TargetList.Num() > 0)
	{
		return false;
	}

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	bool bResult = m_pOwner->GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation,
		FQuat::Identity,
		Channel,
		FCollisionShape::MakeSphere(Radius),
		Params);

	FVector Vec = ActorRigthVector * Range;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = Range * 0.5f + Radius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Blue;
	else
		DrawColor = FColor::Purple;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, Radius,
	//	Rotation, DrawColor, false, 2.f);

	if (bResult)
	{
		for (FOverlapResult Result : OverlapResult)
		{
			if (Result.GetActor() == nullptr)
				continue;

			UE_LOG(LogTemp, Log, TEXT("Search Actor : % s"), *Result.GetActor()->GetName());
			m_TargetList.Add(Result.GetActor());
		}

	}

	return bResult;
}
