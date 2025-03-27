// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_WaterBall.h"
#include "MyAnimInstance.h"

ASkill_WaterBall::ASkill_WaterBall()
{
	// ThunderBall

	static ConstructorHelpers::FClassFinder<AActor> ThunderBall(TEXT("/Game/FXVarietyPack/Blueprints/BP_ky_thunderBall.BP_ky_thunderBall_C"));
	if (ThunderBall.Succeeded())
	{
		m_ThunderBall = ThunderBall.Class;
	}


}

void ASkill_WaterBall::UseSkill(APawn* Pawn, UMyAnimInstance* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == Pawn || nullptr == Anim || nullptr == StatComp)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;

	// AnimInstance Delegate   // 구독하기.
	m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_WaterBall::OnSkillMontageEnded);
	m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_WaterBall::OnSkillMontagePoint);



	m_pAnimInstance->PlaySkillMontage();
	m_pAnimInstance->JumpToSection_Skill(TEXT("Cast"));

	// 마법생성 (임시)
	m_pOwner->GetWorld()->SpawnActor(m_ThunderBall, &m_pOwner->GetTransform());




}

void ASkill_WaterBall::StopSkill()
{
}

void ASkill_WaterBall::PlaySkill()
{

}

void ASkill_WaterBall::CheckSkill()
{
}

void ASkill_WaterBall::OnEffect()
{

}

void ASkill_WaterBall::OnSkillMontageEnded()
{
}

void ASkill_WaterBall::OnSkillMontagePoint()
{
}
