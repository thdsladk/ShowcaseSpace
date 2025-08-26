// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Skill_WaterBall.h"
#include "Header/DataStruct.h"
#include "Animation/AnimInstanceBase.h"
#include "MyStatComponent.h"

ASkill_WaterBall::ASkill_WaterBall()
{
	// ThunderBall

	static ConstructorHelpers::FClassFinder<AActor> ThunderBall(TEXT("/Game/FXVarietyPack/Blueprints/BP_ky_thunderBall.BP_ky_thunderBall_C"));
	if (ThunderBall.Succeeded())
	{
		m_ThunderBall = ThunderBall.Class;
	}


}

void ASkill_WaterBall::InitSkill(const FSkillData* SkillData, APawn* Pawn, UAnimInstanceBase* Anim, UMyStatComponent* StatComp)
{
	if (nullptr == SkillData || nullptr == Pawn || nullptr == Anim || nullptr == StatComp || false == m_bEnableSkill)
		return;
	m_pOwner = Pawn;
	m_pAnimInstance = Anim;
	m_pStatComp = StatComp;
	m_SkillData = SkillData;

	// AnimInstance Delegate   // 구독하기.
	if (m_pAnimInstance->m_OnSkillEnd.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &ASkill_WaterBall::OnSkillMontageEnded);
	if (m_pAnimInstance->m_OnSkillPoint.IsBoundToObject(this) == false)
		m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &ASkill_WaterBall::OnSkillMontagePoint);

}

bool ASkill_WaterBall::UseSkill()
{
	if (nullptr == m_pOwner || nullptr == m_pAnimInstance || nullptr == m_pStatComp)
		return false;

	m_pAnimInstance->PlaySkillMontage(3U);
	m_pAnimInstance->JumpToSection_Skill(3U,TEXT("Cast"));

	// 마법생성 (임시)
	m_pOwner->GetWorld()->SpawnActor(m_ThunderBall, &m_pOwner->GetTransform());

	return true;
}

void ASkill_WaterBall::StopSkill()
{
	m_StopSkill.Broadcast();
}

void ASkill_WaterBall::StoppedSkill(UAnimMontage* Montage, bool bInterrupted)
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

	StopSkill();
}

void ASkill_WaterBall::OnSkillMontagePoint()
{
}
