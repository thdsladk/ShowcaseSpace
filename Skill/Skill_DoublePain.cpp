// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_DoublePain.h"
#include "MyAnimInstance.h"
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

void ASkill_DoublePain::UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;

	// AnimInstance Delegate   // �����ϱ�.
	m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_DoublePain::OnSkillMontageEnded);
	m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_DoublePain::OnSkillMontagePoint);


	// ���⼭ UMyAnimInstace���� �÷��̾� ĳ���Ϳ� ���ӵ� �ִϸ��̼��̴� ... 
	// NPC�� ��� �����ϰ� �Ϸ����ذ��ؾ��Ѵ�.
	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("DoublePain"));


	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_DoublePain, this, &ASkill_DoublePain::PlaySkill, 0.1f, false);

	OnEffect();

}

void ASkill_DoublePain::StopSkill()
{

}

void ASkill_DoublePain::PlaySkill()
{
	
}

void ASkill_DoublePain::CheckSkill()
{
	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	float AttackRange = 150.f;
	float AttackRadius = 200.f;
	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

	if (m_pOwner->GetWorld() == nullptr)
		UE_LOG(LogTemp, Log, TEXT(" ~ World is Null ~"));
	bool bResult = m_pOwner->GetWorld()->OverlapMultiByChannel(
		OUT OverlapResult,
		ActorLocation + ActorRigthVector * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = ActorRigthVector * AttackRange;
	FVector Center = ActorLocation + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
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

			int32 Damage = m_pStatComp->GetAttack() * 20;
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
	
}

void ASkill_DoublePain::OnSkillMontagePoint()
{
	CheckSkill();
}

