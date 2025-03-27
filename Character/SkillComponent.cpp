// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "MyTest_TopDownCharacter.h"
#include "MyAnimInstance.h"
#include "MyStatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Math/UnrealMathUtility.h"


#include "Kismet/GameplayStatics.h"

// 스킬 클래스들
#include "SKill/SkillCommandBase.h"
#include "Skill/Skill_DoublePain.h"
#include "Skill/Skill_Stampede.h"
#include "Skill/Skill_FinalHit.h"
#include "Skill/Skill_WaterBall.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SwordTrail(TEXT("/Script/Niagara.NiagaraSystem'/Game/TopDown/Effect/VFX_SwordTrail.VFX_SwordTrail'"));
	//if (SwordTrail.Succeeded())
	//{
	//	m_SwordTrail = SwordTrail.Object;
	//
	//}

	/*
	m_pDust = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dust"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Dust(TEXT("/Script/Niagara.NiagaraSystem'/Game/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_Sand_Surface.PSN_Sand_Surface'"));
	if (Dust.Object)
	{
		
		m_pDust->SetAsset(Dust.Object);
		m_pDust->bAutoActivate = false;
		m_pDust->Deactivate();
		m_pDust->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.3f));
	}
	*/
}

// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pOwner = Cast<AMyTest_TopDownCharacter>(GetOwner());
	m_pAnimInstance = Cast<UMyAnimInstance>(m_pOwner.Get()->GetMesh()->GetAnimInstance());
	m_pStatComp = &(m_pOwner->GetStatComponent());

	// Nullptr Check
	if (m_pOwner == nullptr || m_pAnimInstance == nullptr || m_pStatComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT(" Character or AnimInstance or StatComponent is nullptr !!!"));
		return;
	}
	

	// AnimInstance Delegate   // 구독하기.
	m_pAnimInstance->m_OnSkillEnd.AddUObject(this, &USkillComponent::OnSkillMontageEnded);
	m_pAnimInstance->m_OnSkillPoint.AddUObject(this, &USkillComponent::OnSkillMontagePoint);

	// 임시로 스킬 리스트에 스킬 객체를 만들어서 넣는다. 

	m_SkillList.SetNum(ESkill::END);
	m_SkillList[static_cast<int32>(ESkill::Skill_Q)] = NewObject<ASkill_DoublePain>();
	m_SkillList[static_cast<int32>(ESkill::Skill_W)] = NewObject<ASkill_Stampede>();
	m_SkillList[static_cast<int32>(ESkill::Skill_E)] = NewObject<ASkill_FinalHit>();
	m_SkillList[static_cast<int32>(ESkill::Skill_R)] = NewObject<ASkill_WaterBall>();


}

void USkillComponent::Click_Q()
{
	if (m_pOwner->GetState() == static_cast<uint8>(EBehaviorState::Idle))
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Casting));
	else
		return;

	m_CurrentSkill = ESkill::Skill_Q;
	m_SkillList[m_CurrentSkill]->UseSkill(m_pOwner.Get(),m_pAnimInstance.Get(),m_pStatComp.Get());

	
	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_W()
{
	if (m_pOwner->GetState() == static_cast<uint8>(EBehaviorState::Idle))
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Casting));
	else
		return;

	m_CurrentSkill = ESkill::Skill_W;
	m_SkillList[m_CurrentSkill]->UseSkill(m_pOwner.Get(), m_pAnimInstance.Get(), m_pStatComp.Get());

	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_E()
{
	if (m_pOwner->GetState() == static_cast<uint8>(EBehaviorState::Idle))
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Casting));
	else
		return;

	m_CurrentSkill = ESkill::Skill_E;
	m_SkillList[m_CurrentSkill]->UseSkill(m_pOwner.Get(), m_pAnimInstance.Get(), m_pStatComp.Get());

	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_R()
{
	if (m_pOwner->GetState() == static_cast<uint8>(EBehaviorState::Idle))
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Casting));
	else
		return;

	m_CurrentSkill = ESkill::Skill_R;
	m_SkillList[m_CurrentSkill]->UseSkill(m_pOwner.Get(), m_pAnimInstance.Get(), m_pStatComp.Get());

	// 제어권 해제
	m_pOwner->DisableInput(Cast<APlayerController>(m_pOwner->GetController()));

}

void USkillComponent::Click_Btn(uint8 Btn)
{
	switch (Btn)
	{
	case ESkill::Skill_Q :
	{
		Click_Q();
		break;
	}
	case ESkill::Skill_W:
	{
		Click_W();
		break;
	}
	case ESkill::Skill_E:
	{
		Click_E();
		break;
	}
	case ESkill::Skill_R:
	{
		Click_R();
		break;
	}
	case ESkill::END:
	{
		break;
	}
	}
}

void USkillComponent::Cancel()
{
	// 상태가 취소될때. 
	m_CurrentSkill = ESkill::END;
}

void USkillComponent::OnSkillMontageEnded()
{

	m_pOwner->EnableInput(Cast<APlayerController>(m_pOwner->GetController()));

	//m_pAnimInstance->Montage_Stop(0.f);

	switch (m_CurrentSkill)
	{
	case USkillComponent::Skill_Q:
	{

		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Idle));
		break;
	}
	case USkillComponent::Skill_W:
	{
		
		break;
	}
	case USkillComponent::Skill_E:
	{
		//m_CurrentSkill = ESkill::END;
		//m_pOwner->SetState(EBehaviorState::Idle);
		break;
	}
	case USkillComponent::Skill_R:
	{
		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Idle));
		break;
	}
	case USkillComponent::END:
	{
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Idle));
		break;
	}
	default:
	{
		m_CurrentSkill = ESkill::END;
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Idle));
		break;
	}
	}


}

void USkillComponent::OnSkillMontagePoint()
{

	switch (m_CurrentSkill)
	{
	case USkillComponent::Skill_Q:
	{
		break;
	}
	case USkillComponent::Skill_W:
	{

		m_CurrentSkill = ESkill::END;		// END로 전환해서  애니메이션이 끝났을때는 
		break;								// OnSkillMontateEnded 에서 Skill_W가 두번 실행 안되도록 한다.
	}
	case USkillComponent::Skill_E:
	{

		break;
	}
	case USkillComponent::Skill_R:
	{

		break;
	}
	case USkillComponent::END:
	{

		break;
	}
	default:
	{

		break;
	}
	}

}

// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...



}


/*

/Script/Engine.ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Ultimate/FX/P_MeleeTrails_Ultimate.P_MeleeTrails_Ultimate'

*/