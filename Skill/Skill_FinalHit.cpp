// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_FinalHit.h"
#include "Header/DataStruct.h"
#include "Engine/DamageEvents.h"
#include "Animation/AnimInstanceBase.h"
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

void ASkill_FinalHit::InitSkill(const FSkillData* SkillData, APawn* Pawn, UAnimInstanceBase* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == SkillData || nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;
	m_SkillData = SkillData;

	// AnimInstance Delegate   // �����ϱ�.
	
	// �������� ���̸� �ȵȴ�. ��ų�� ���ε� �Ǿ���Ѵ�. 
	//if(m_pAnimInstance->m_OnAttackEnd.IsBoundToObject(this) == false)
	//	m_pAnimInstance->m_OnAttackEnd.AddUObject(this, &ASkill_FinalHit::PlaySkill);
	if(m_pAnimInstance->m_OnSkillEnd.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_FinalHit::OnSkillMontageEnded);
	if(m_pAnimInstance->m_OnSkillPoint.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_FinalHit::OnSkillMontagePoint);
	if (m_pAnimInstance->OnMontageEnded.IsAlreadyBound(this, &ASkill_FinalHit::StoppedSkill) == false)
		m_pAnimInstance->OnMontageEnded.AddDynamic(this, &ASkill_FinalHit::StoppedSkill);

	

}

bool ASkill_FinalHit::UseSkill()
{
	if (nullptr == m_pOwner || nullptr == m_pAnimInstance || nullptr == m_pStatComp || false == m_bEnableSkill)
		return false;
	
	// Cost
	if (m_pStatComp->GetMP() < m_SkillData->Cost.Y) { return false; }
	else { m_pStatComp->AddMP(m_SkillData->Cost.Y * -1); }


	m_pAnimInstance->PlaySkillMontage(2U);
	m_pAnimInstance->JumpToSection_Skill(2U,TEXT("LevelStart"));

	// ��ų ���� 
	SearchActor_Sphere(1.f, 1200.f, ECollisionChannel::ECC_GameTraceChannel2);
	m_bGhostTrail = true;
	
	// OnSkillTick 
	int32 Rate = 0.1f;
	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_SkillTimer, this, &ASkill_FinalHit::OnEffect, Rate, true);

	// CoolDown
	m_bEnableSkill = false;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {m_bEnableSkill = true; });
	if (m_pOwner->GetWorld()->GetTimerManager().IsTimerActive(m_CoolDownTimer) == false)
	{
		m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_CoolDownTimer, TimerDelegate, m_SkillData->CoolDown, false);
	}

	// ������ ���� On
	CastChecked<ACharacterBase>(m_pOwner)->SetInvincible(true);
	
	return true;
}

void ASkill_FinalHit::StopSkill()
{
	// ������ ���� Off
	CastChecked<ACharacterBase>(m_pOwner)->SetInvincible(false);

	m_bGhostTrail = false;
	m_StopSkill.Broadcast();
}

void ASkill_FinalHit::StoppedSkill(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == true)
	{
		// ������ ���� Off
		CastChecked<ACharacterBase>(m_pOwner)->SetInvincible(false);

		m_bGhostTrail = false;
		m_StopSkill.Broadcast();
	}

}

void ASkill_FinalHit::PlaySkill()
{
	// �ѹ��� ���� ��ƾ 

// ����Ʈ Ȯ��
	if (m_TargetList.IsEmpty())
	{
		//m_pOwner->EnableInput(Cast<APlayerController>(m_pOwner->GetController()));
		CastChecked<ACharacterBase>(m_pOwner)->SetState(static_cast<uint8>(EBehaviorState::Battle));
		CastChecked<ACharacterBase>(m_pOwner)->SetNextState(static_cast<uint8>(EBehaviorState::Battle));
		m_bGhostTrail = false;
		// OffSkillTick 
		m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_SkillTimer);

		StopSkill();
		return;
	}

	AActor* pTarget = m_TargetList.Last().Get();
	if (pTarget == nullptr)
	{
		StopSkill();
		return;
	}

	// �̵� 
	m_pOwner->SetActorLocation(pTarget->GetActorLocation() + (pTarget->GetActorForwardVector() * 50.f));

	FVector LookVector = -(pTarget->GetActorForwardVector());
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	m_pOwner->SetActorRotation(TargetRot);

	/*  
	(���� �ʿ�)
	
	1. ȣ�� ������ 1) ����[Casting] 2)����[Attack] 3) ���� ���� ������ ȣ��Ǵµ� 
			�׷��� 2)���� �׳� ĵ���Ǿ� ������ ������ �����.    ȣ�� ������ �����̴�. 

	*/
	// ����			
	m_pAnimInstance->PlayMontage(static_cast<uint8>(EMontageList::AttackMontage));
	m_pAnimInstance->JumpToSection(m_pAnimInstance->GetAttackMontageName(CastChecked<ACharacterBase>(m_pOwner)->GetAttackIndex()), static_cast<uint8>(EMontageList::AttackMontage));
	CheckSkill();
	
	// ���
	m_TargetList.Remove(pTarget);

}

void ASkill_FinalHit::CheckSkill()
{
	FBaseStatusData Status = m_pStatComp->GetTotalStat();

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	Params.AddIgnoredActor(this);

	FVector Vec = GetActorForwardVector() * Status.AttackRange;
	FVector Center = GetActorLocation() + Vec * 1.0f;
	float HalfHeight = Status.AttackRange * 0.5f + Status.AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorRightVector()).ToQuat();


	bool bResult = GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		GetActorLocation() + Vec,
		Rotation,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(Status.AttackRadius, HalfHeight),
		Params);


	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, Status.AttackRadius,
		Rotation, DrawColor, false, 2.f);


	// �ߺ� ���Ÿ� ���ؼ� �ѹ� �ɷ��ش�.
	TArray<AActor*> UniqueResult;
	for (FOverlapResult Src : OverlapResult)
	{
		UniqueResult.AddUnique(Src.GetActor());
	}

	if (bResult)
	{
		for (AActor* Result : UniqueResult)
		{

			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *Result->GetName());

			FDamageEvent DamageEvent;
			Result->TakeDamage(Status.Attack, DamageEvent, m_pOwner->GetController(), this);

		}
	}
}

void ASkill_FinalHit::OnEffect()
{
	if (m_bGhostTrail == true)
	{
		// GhostTrail Effect
		FVector Location = m_pOwner->GetActorLocation();

		AGhostTrail* GhostTrail = Cast<AGhostTrail>(m_pOwner->GetWorld()->SpawnActor(m_pGhostTrail, &Location));
		GhostTrail->SetupGhostTrail(CastChecked<ACharacter>(m_pOwner));
		GhostTrail->BeginGhostTrailEffect();
	}

}

void ASkill_FinalHit::OnSkillMontageEnded()
{

}

void ASkill_FinalHit::OnSkillMontagePoint()
{
	if (m_bGhostTrail == true)
	{
		PlaySkill();
	}
}

bool ASkill_FinalHit::SearchActor_Sphere(float Range, float Radius, ECollisionChannel Channel)
{
	if (m_TargetList.Num() > 0 )
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
			if (m_TargetList.Num() < m_MaxCount)
			{
				// �ߺ� ���Ÿ� ���ؼ� �ѹ� �ɷ��ش�.
				m_TargetList.AddUnique(Result.GetActor());
			}
			else
			{
				// ���뷮�� �Ѿ�� Return
				return bResult;
			}
		}

	}

	return bResult;
}
