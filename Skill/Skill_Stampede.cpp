// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_Stampede.h"
#include "MyAnimInstance.h"
#include "MyStatComponent.h"

//이 부분은 조금 더 추상화해서 CharacterBase로 변경해야한다.
#include "MyTest_TopDownCharacter.h"

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

void ASkill_Stampede::UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;

	// AnimInstance Delegate   // 구독하기.
	m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_Stampede::OnSkillMontageEnded);
	m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_Stampede::OnSkillMontagePoint);


	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("Stampede"));

	//(임시) 플레이어캐릭터에 종속 되는 부분... 
	CastChecked<AMyTest_TopDownCharacter>(m_pOwner)->SetSprint(2000.f, 50.f);		// 캐릭터 이동속도
	m_bStampede = true;

	// Effect
	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_Stampede, this, &ASkill_Stampede::OnEffect, 0.1f, true);

	// OnSkillTick 
	int32 Rate = 1.f;
	m_pOwner->GetWorld()->GetTimerManager().SetTimer(m_Timer_Tick, this, &ASkill_Stampede::SkillTick, Rate, true);

}

void ASkill_Stampede::StopSkill()
{
	m_bStampede = false;

	m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);
	CastChecked<AMyTest_TopDownCharacter>(m_pOwner)->StopSprint();
}

void ASkill_Stampede::PlaySkill()
{
	FVector WorldDirection = m_pOwner->GetActorForwardVector().GetSafeNormal();
	m_pOwner->AddMovementInput(WorldDirection, 1.0f, false);
}

void ASkill_Stampede::CheckSkill()
{
}

void ASkill_Stampede::OnEffect()
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

void ASkill_Stampede::OnSkillMontageEnded()
{
	//(임시)이렇게 플레이어캐릭터에 종속되면 안된다. 
	CastChecked<AMyTest_TopDownCharacter>(m_pOwner)->StopSprint();
	m_bStampede = false;
	PlaySkill_Stampede_Knockup();


	// OffSkillTick 
	m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Tick);

}

void ASkill_Stampede::OnSkillMontagePoint()
{
	CheckSkill_Stampede_Knockup();
}

void ASkill_Stampede::SkillTick()
{
	if (m_bStampede)
	{
		PlaySkill();
	}
}

void ASkill_Stampede::PlaySkill_Stampede_Knockup()
{
	//m_CurrentSkill = ESkill::END;	// END상태로 돌리면 위험할듯하니 조정하자 .

	m_pOwner->GetWorld()->GetTimerManager().ClearTimer(m_Timer_Stampede);

	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("Stampede_Knockup"));


}

void ASkill_Stampede::CheckSkill_Stampede_Knockup()
{
	TArray<FOverlapResult> OverlapResult;
	FCollisionQueryParams Params(NAME_None, false, m_pOwner.Get());

	float AttackRange = 150.f;
	float AttackRadius = 200.f;
	FVector ActorLocation = m_pOwner->GetActorLocation();
	FVector ActorRigthVector = m_pOwner->GetActorRightVector();

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
			Result.GetActor()->TakeDamage(m_pStatComp->GetAttack(), DamageEvent, m_pOwner->GetController(), m_pOwner.Get());

		}

	}
}

void ASkill_Stampede::Effect_Thunder()
{
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

