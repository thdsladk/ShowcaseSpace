// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_DoublePain.h"
#include "Header/DataStruct.h"
#include "Animation/AnimInstanceBase.h"
#include "MyStatComponent.h"
#include "Engine/DamageEvents.h"


// Sets default values
ASkill_DoublePain::ASkill_DoublePain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
		��ų ��ü�� ���忡�� Spawn���� �ʾƼ� 
		GetWorld()�� ȣ���Ҽ� ����. 
		�׷��� m_pOwneró�� ���忡 ��ȯ�� ��ü�� GetWorld()�� ����ؾ��Ѵ�. 
	

		�ϴ� BeginPlay()�� Tick() �� ���� ���ؼ��� ���忡 �����ؾ��ҵ��ϴ� . 

		UObject �� AActor�� ��� ����ϴ°� ������ ����� ������
		������� ��ȣ�ۿ��� ����� ������ ����ؼ� AActor�� ��ӹ��� ( ���� m_pOwner�� ���带 ����ϰ��ֱ��ϴ�. ) 

	*/
}

void ASkill_DoublePain::InitSkill(const FSkillData* SkillData, APawn* Pawn, UAnimInstanceBase* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == SkillData || nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;
	m_SkillData = SkillData;

	// AnimInstance Delegate   // �����ϱ�.
	if (m_pAnimInstance->m_OnSkillEnd.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_DoublePain::OnSkillMontageEnded);
	if (m_pAnimInstance->m_OnSkillPoint.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_DoublePain::OnSkillMontagePoint);


}

bool ASkill_DoublePain::UseSkill()
{
	if (nullptr == m_pOwner || nullptr == m_pAnimInstance || nullptr == m_pStatComp || false == m_bEnableSkill)
		return false;
	
	// Cost
	if (m_pStatComp->GetMP() < m_SkillData->Cost.Y) { return false; }
	else { m_pStatComp->AddMP(m_SkillData->Cost.Y * -1); }

	// UAnimInstanceBase ������� ������. 
	m_pAnimInstance->PlaySkillMontage(0U);
	m_pAnimInstance->JumpToSection_Skill(0U,TEXT("DoublePain"));

	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_DoublePain, this, &ASkill_DoublePain::PlaySkill, 0.1f, false);
	
	// CoolDown
	m_bEnableSkill = false;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {m_bEnableSkill = true; });
	if (m_pOwner->GetWorld()->GetTimerManager().IsTimerActive(m_CoolDownTimer) == false)
	{
		// ���⼭ ������ ������ , ��ٿ �����ϰ� 
		m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_CoolDownTimer,TimerDelegate, m_SkillData->CoolDown, false);
	}

	OnEffect();

	return true;
}

void ASkill_DoublePain::StopSkill()
{
	m_StopSkill.Broadcast();
}

void ASkill_DoublePain::StoppedSkill(UAnimMontage* Montage, bool bInterrupted)
{
}

void ASkill_DoublePain::PlaySkill()
{
	
}

void ASkill_DoublePain::CheckSkill()
{
	FBaseStatusData Status = m_pStatComp->GetTotalStat();

	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());


	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	if (m_pOwner->GetWorld() == nullptr)
		UE_LOG(LogTemp, Log, TEXT(" ~ World is Null ~"));
	bool bResult = m_pOwner->GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation + ActorRigthVector * Status.AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Status.AttackRadius),
		Params);

	FVector Vec = ActorRigthVector * Status.AttackRange;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = Status.AttackRange * 0.5f + Status.AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(m_pOwner->GetWorld(), Center, HalfHeight, AttackRadius,
	//	Rotation, DrawColor, false, 2.f);

	if (bResult)
	{
		for (FOverlapResult Result : OverlapResult)
		{
			if (Result.GetActor() == nullptr)
				continue;

			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *Result.GetActor()->GetName());
			FDamageEvent DamageEvent;

			int32 SkillDamage_Ratio = 20;
			int32 Damage = (Status.Attack * SkillDamage_Ratio);
			Result.GetActor()->TakeDamage(Damage, DamageEvent, m_pOwner->GetController(), m_pOwner.Get());

		}

	}
}

void ASkill_DoublePain::OnEffect()
{
	// SwordTrail Effect 

//UNiagaraFunctionLibrary::SpawnSystemAttached(
//	m_SwordTrail,  // ����� ���� ȿ���� Niagara ���� �̸��� �־��ּ���.
//	m_pOwner->GetMesh(),
//	FName(TEXT("FX_Trail_L_01")),
//	FVector::ZeroVector,
//	FRotator::ZeroRotator,
//	EAttachLocation::KeepRelativeOffset,
//	true
//);
//
//UNiagaraFunctionLibrary::SpawnSystemAttached(
//	m_SwordTrail,  // ����� ���� ȿ���� Niagara ���� �̸��� �־��ּ���.
//	m_pOwner->GetMesh(),
//	FName(TEXT("FX_Trail_R_01")),
//	FVector::ZeroVector,
//	FRotator::ZeroRotator,
//	EAttachLocation::KeepRelativeOffset,
//	true
//);


}

void ASkill_DoublePain::OnSkillMontageEnded()
{
	StopSkill();
}

void ASkill_DoublePain::OnSkillMontagePoint()
{
	CheckSkill();
}


