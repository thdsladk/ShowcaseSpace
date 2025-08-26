// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_Stampede.h"
#include "Header/DataStruct.h"
#include "Animation/AnimInstanceBase.h"
#include "MyStatComponent.h"

#include "Character/CharacterBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"



ASkill_Stampede::ASkill_Stampede()
{
	static ConstructorHelpers::FClassFinder<AActor> Thunder(TEXT("/Game/FXVarietyPack/Blueprints/BP_ky_lightning1.BP_ky_lightning1_C"));
	if (Thunder.Succeeded())
	{
		m_ThunderList.Add(Thunder.Class);

	}

	static ConstructorHelpers::FClassFinder<AActor> Thunder_Middle(TEXT("/Game/FXVarietyPack/Blueprints/BP_ky_lightning1.BP_ky_lightning2_C"));
	if (Thunder_Middle.Succeeded())
	{
		m_ThunderList.Add(Thunder_Middle.Class);
	}

	// Niagara System ( Particle ) 
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Dust(TEXT("/Game/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Sand_Surface.PSN_Sand_Surface"));
	if (Dust.Succeeded())
	{
		m_pDust = Dust.Object;
	}
}

void ASkill_Stampede::InitSkill(const FSkillData* SkillData, APawn* Pawn, UAnimInstanceBase* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == SkillData || nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;
	m_SkillData = SkillData;

	// AnimInstance Delegate   // 구독하기.
	if (m_pAnimInstance->m_OnSkillEnd.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_Stampede::OnSkillMontageEnded);
	if (m_pAnimInstance->m_OnSkillPoint.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_Stampede::OnSkillMontagePoint);
	if (m_pAnimInstance->OnMontageEnded.IsAlreadyBound(this, &ASkill_Stampede::StoppedSkill) == false)
		m_pAnimInstance->OnMontageEnded.AddDynamic(this, &ASkill_Stampede::StoppedSkill);

}

bool ASkill_Stampede::UseSkill()
{
	if (nullptr == m_pOwner || nullptr == m_pAnimInstance || nullptr == m_pStatComp || false == m_bEnableSkill)
		return false;

	// Cost
	if (m_pStatComp->GetMP() < m_SkillData->Cost.Y) { return false; }
	else { m_pStatComp->AddMP(m_SkillData->Cost.Y * -1); }


	m_pAnimInstance->PlaySkillMontage(1U);
	m_pAnimInstance->JumpToSection_Skill(1U,TEXT("Stampede"));

	//(임시) 플레이어캐릭터에 종속 되는 부분... 
	CastChecked<ACharacterBase>(m_pOwner)->Sprint(3.f, 3.f,false);		// 캐릭터 이동속도
	m_bStampede = true;

	// Effect
	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_Stampede, this, &ASkill_Stampede::OnEffect, 0.1f, true);
	
	// CoolDown
	m_bEnableSkill = false;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {m_bEnableSkill = true; });
	if (m_pOwner->GetWorld()->GetTimerManager().IsTimerActive(m_CoolDownTimer) == false)
	{
		m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_CoolDownTimer, TimerDelegate, m_SkillData->CoolDown, false);
	}

	// OnSkillTick 
	m_bCanEverTick = true;

	return true;
}

void ASkill_Stampede::StopSkill()
{
	m_bStampede = false;
	m_bCanEverTick = false;

	m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);
	CastChecked<ACharacterBase>(m_pOwner)->StopSprint(false);

	m_StopSkill.Broadcast();
}

void ASkill_Stampede::StoppedSkill(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == true)
	{
		m_bStampede = false;
		m_bCanEverTick = false;

		m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);
		CastChecked<ACharacterBase>(m_pOwner)->StopSprint(false);

		m_StopSkill.Broadcast();
	}
}

void ASkill_Stampede::PlaySkill()
{
	if (m_bStampede)
	{
		FVector ForwardDirection = m_pOwner->GetActorForwardVector().GetSafeNormal();
		m_pOwner->AddMovementInput(ForwardDirection, 1.0f, false);
	}
}

void ASkill_Stampede::CheckSkill()
{
	FBaseStatusData Status = m_pStatComp->GetTotalStat();

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	float SkillAttackRange = Status.AttackRange;
	float SkillAttackRadius = Status.AttackRadius + 50.f;
	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	bool bResult = m_pOwner->GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation + ActorRigthVector * SkillAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SkillAttackRadius),
		Params);

	FVector Vec = ActorRigthVector * SkillAttackRange;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = SkillAttackRange * 0.5f + SkillAttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(m_pOwner->GetWorld(), Center, HalfHeight, SkillAttackRadius,
	//	Rotation, DrawColor, false, 2.f);

	if (bResult)
	{
		for (FOverlapResult Result : OverlapResult)
		{
			if (Result.GetActor() == nullptr)
				continue;

			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *Result.GetActor()->GetName());
			FDamageEvent DamageEvent;

			int32 SkillDamage_Ratio = 2;
			int32 Damage = (Status.Attack * SkillDamage_Ratio);
			Result.GetActor()->TakeDamage(Damage, DamageEvent, m_pOwner->GetController(), m_pOwner.Get());

		}

	}
}

void ASkill_Stampede::OnEffect()
{
	Effect_Thunder();
	Effect_Dust();
}

void ASkill_Stampede::OnSkillMontageEnded()
{
	if (m_bStampede == true)
	{
		// 질주 종료 시점
		CastChecked<ACharacterBase>(m_pOwner)->StopSprint(false);
		m_bStampede = false;

		// 
		m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);
	}
	else
	{
		// 최종 종료 시점
		StopSkill();
	}
}

void ASkill_Stampede::OnSkillMontagePoint()
{
	CheckSkill();
}

void ASkill_Stampede::SkillTick(float DeltaTime)
{
	Super::SkillTick(DeltaTime);

	PlaySkill();

}

void ASkill_Stampede::Effect_Thunder()
{
	int32 nMax = 20;
	int32 nMin = 5;
	for (auto elem : m_ThunderList)
	{
		if (nMax < nMin)
			nMax = nMin;
		int32 Count = FMath::RandRange(nMin, nMax - nMin);
		nMax -= Count;

		for (int32 i = 0; i < Count; i++)
		{
			float fRangeX = FMath::RandRange(-10.f, 10.f) * 40.f;
			float fRangeY = FMath::RandRange(-10.f, 10.f) * 40.f;

			FTransform Transform = m_pOwner->GetTransform();
			Transform.SetLocation(Transform.GetLocation() + FVector(fRangeX, fRangeY, 0.f));
			m_pOwner->GetWorld()->SpawnActor(elem, &Transform);
		}

	}
}

void ASkill_Stampede::Effect_Dust()
{
	// Dust Effect 

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		m_pDust,  // 사용할 먼지 효과의 Niagara 에셋 이름을 넣어주세요.
		CastChecked<ACharacter>(m_pOwner)->GetMesh(),
		FName(TEXT("foot_l")),  // 이름은 필요에 따라 변경 가능
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		m_pDust,  // 사용할 먼지 효과의 Niagara 에셋 이름을 넣어주세요.
		CastChecked<ACharacter>(m_pOwner)->GetMesh(),
		FName(TEXT("foot_R")),  // 이름은 필요에 따라 변경 가능
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);
}

