// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "Character/CharacterBase.h"
#include "Animation/AnimInstanceBase.h"
#include "Character/MyTest_TopDownPlayerController.h"
#include "CharacterComponent/MyStatComponent.h"
#include "UI/MyHUD.h"
#include "Data/MyGameSubsystem.h"

//#include "GameFramework/CharacterMovementComponent.h"
//#include "Camera/CameraComponent.h"
//#include "GameFramework/SpringArmComponent.h"

//#include "Math/UnrealMathUtility.h"

// 스킬 클래스들
#include "SKill/SkillCommandBase.h"
#include "Skill/Skill_DoublePain.h"
#include "Skill/Skill_Stampede.h"
#include "Skill/Skill_FinalHit.h"
#include "Skill/Skill_WaterBall.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent()
	: m_PreBehaviorState(EBehaviorState::Idle)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

}

// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pOwner = CastChecked<ACharacterBase>(GetOwner());
	m_pAnimInstance = CastChecked<UAnimInstanceBase>(m_pOwner.Get()->GetMesh()->GetAnimInstance());
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
	
	// MyHUD Skill Widget Delegate // 구독하기
	AMyTest_TopDownPlayerController* PC = Cast<AMyTest_TopDownPlayerController>(m_pOwner->GetController());
	if (PC != nullptr)
	{
		UMyHUD* MyHUD = Cast<UMyHUD>(PC->GetCurrentWidget());
		if (MyHUD != nullptr)
		{
			MyHUD->m_OnSkill.AddUObject(this, &USkillComponent::UseSkill);
		}
	}

	// (임시) 플레이어 캐릭터라면 스킬 테이블에서 스킬 셋을 가져온다.
	if (m_pOwner->IsPlayerCharacter() == true)
	{
		m_SkillList.Init(nullptr, static_cast<int32>(ESkill::End));

		UMyGameSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMyGameSubsystem>();

		if (Subsystem != nullptr)
		{
			// 언제든 고유 ID로 스킬을 장착가능.
			for (int32 i =0;i<static_cast<int32>(ESkill::End);++i)
			{
				const FSkillData* SkillData = Subsystem->GetSkillData(i);
				
				UClass* Class = SkillData->SkillClass.LoadSynchronous();
				m_SkillList[i] = NewObject<ASkillCommandBase>(this, Class,*(SkillData->Name));

				// Bind Skill
				m_SkillList[i]->m_StopSkill.AddUObject(this, &USkillComponent::CancelSkill);
				m_SkillList[i]->InitSkill(SkillData,m_pOwner.Get(), m_pAnimInstance.Get(), m_pStatComp.Get());
			}
		}


	}

}

void USkillComponent::UseSkill(uint8 Btn)
{
	if (m_pOwner->GetState() == EBehaviorState::Idle ||
		m_pOwner->GetState() == EBehaviorState::Battle)
	{
		m_PreBehaviorState = m_pOwner->GetState();
		m_pOwner->SetState(static_cast<uint8>(EBehaviorState::Casting));
	}
	else
		return;

	if (m_CurrentSkill == static_cast<ESkill>(Btn))
		return;

	// 제어권 해제
	AMyTest_TopDownPlayerController* PC = Cast<AMyTest_TopDownPlayerController>(m_pOwner->GetController());
	PC->SetGamePlayControllable(false);

	m_CurrentSkill = static_cast<ESkill>(Btn);
	if (m_SkillList[Btn] != nullptr)
	{
		if (m_SkillList[Btn]->UseSkill() == false)
		{
			CancelSkill();
		}
		else
		{
			m_OnSkill.Broadcast(Btn);
		}

	}
	else
	{
		return;
	}
}

void USkillComponent::CancelSkill()
{
	// 상태가 취소될때. 
	m_CurrentSkill = ESkill::End;
	m_EndSkill.Broadcast(static_cast<uint8>(m_PreBehaviorState));

	// 제어권 회복
	AMyTest_TopDownPlayerController* PC = Cast<AMyTest_TopDownPlayerController>(m_pOwner->GetController());
	PC->SetGamePlayControllable(true);
}

void USkillComponent::OnSkillMontageEnded()
{
	switch (m_CurrentSkill)
	{
	case ESkill::Skill_Q:
	{
		break;
	}
	case ESkill::Skill_W:
	{
		break;
	}
	case ESkill::Skill_E:
	{

		break;
	}
	case ESkill::Skill_R:
	{
		break;
	}
	case ESkill::End:
	{

		break;
	}
	default:
	{

		break;
	}
	}


}

void USkillComponent::OnSkillMontagePoint()
{

	switch (m_CurrentSkill)
	{
	case ESkill::Skill_Q:
	{
		break;
	}
	case ESkill::Skill_W:
	{

		break;								
	}
	case ESkill::Skill_E:
	{

		break;
	}
	case ESkill::Skill_R:
	{

		break;
	}
	case ESkill::End:
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

	for (auto Elem : m_SkillList)
	{
		if (Elem != nullptr)
		{
			if (Elem->IsCanEverTick() == true)
			{
				Elem->SkillTick(DeltaTime);
			}
		}
	}
	

}
