// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_SkillBase.h"
#include "GameFramework/Character.h"
#include "Data/DA_SkillData.h"
#include "AbilitySystemComponent.h"
#include "Actor/IndicatorBase.h"
#include "Components/StaticMeshComponent.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/JWGameplayTag.h"
#include "TargetActor/TA_Base.h"
#include "TargetActor/TA_Skill.h"
#include "AbilityTask/AT_WaitTargetData_Confirm.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Attribute/CharacterSkillAttributeSet.h"

#include "MotionWarpingComponent.h"
#include "CharacterComponents/TargetSystemComponent.h"

// Task 관련 헤더
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"


// 플레이어 전용 코드 
#include "Interface/PlayerControllerInterface.h"

/*
	내부에서 플레이어 전용 코드를 꽤 많이 쓰고 있어서 범용 스킬 베이스로 만들려면 수정이 많이 필요하다. 

	스킬 어빌리티 역할 : 1. 태스크 생성 및 관리 2. Indicator 생성 및 관리 3.TargetSystem의 세팅을 가져와서 적용하기 [ MotionWarping에 적용 ] 


*/

UGA_SkillBase::UGA_SkillBase() :
	m_IndicatorClass(nullptr),
	m_OriginMovementMode(0U),
	m_CurrentCombatMode(0U),
	m_bPlaySkill(false),
	m_HasMovement(false),
	m_SkillSpeedRatio(1.f),
	m_TargetSystemComp(nullptr),
	m_ConfirmationType(EGameplayTargetingConfirmation::Instant)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_SkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Super::CommitAbility(Handle, ActorInfo, ActivationInfo);		// GA_ActionAbilityBase에서 호출 하고 있다.

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Character->GetController());
	m_CurrentCombatMode = PCI->GetCombatMode();
	
	// Target System Component 캐싱.
	m_TargetSystemComp = Character->FindComponentByClass<UTargetSystemComponent>();
	if (m_TargetSystemComp == nullptr) return;

	// 스킬 타겟데이터 등록 
	m_TargetSystemComp->SetSkillTargetData(SkillTargetData);
	m_TargetSystemComp->SetSkillTargetActorClass(m_SkillTargetActorClass);

	// Init Skill Effect 적용
	if (m_InitSkillEffect != nullptr)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(m_InitSkillEffect, 1.f, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}


	// 타겟액터를 생성하기 위한 태스크 생성
	UAT_WaitTargetData_Confirm* RecticleAndCornfirm = Cast< UAT_WaitTargetData_Confirm>(UAT_WaitTargetData_Confirm::WaitTargetData(this,FName("TAT_WaitTargetData_Confirm"), m_ConfirmationType,m_TargetActorClass));
	if (RecticleAndCornfirm != nullptr)
	{
		RecticleAndCornfirm->ValidData.AddDynamic(this, &UGA_SkillBase::PlaySkill_TargetData);
		RecticleAndCornfirm->Cancelled.AddDynamic(this, &UGA_SkillBase::CancelSkill_TargetData);
		RecticleAndCornfirm->ReadyForActivation();
	}

}

void UGA_SkillBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 발동된 상태에서 한번더 키입력을 할 경우 스킬이 시전되도록 구성 . 
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

}

void UGA_SkillBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility, true);
}

void UGA_SkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 몽타주 정지.
	MontageStop(0.25f);
	
	// 태스크와 사용한 자원은 항상 제거해주기. 

	if(m_IndicatorTask != nullptr)
	{
		m_IndicatorTask->OnFinish.Clear();

		m_IndicatorTask->EndTask();
		m_IndicatorTask = nullptr;
	}

	if(m_PlaySkillTask != nullptr)
	{
		m_PlaySkillTask->OnCompleted.Clear();
		m_PlaySkillTask->OnInterrupted.Clear();

		m_PlaySkillTask->EndTask();
		m_PlaySkillTask = nullptr;
	}

	// 스킬 재생 플래그 초기화
	m_bPlaySkill = false;

	// MotionWarping 타겟 제거
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();
	if (MotionWarpingComp != nullptr)
	{
		MotionWarpingComp->RemoveWarpTarget(FName(TEXT("TargetActor")));
	}

	// 타게팅 종료
	UTargetSystemComponent* TSC = Character->FindComponentByClass<UTargetSystemComponent>();
	if(TSC != nullptr)
	{
		TSC->EndTargeting();
	}
}

void UGA_SkillBase::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_SkillBase::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
}

void UGA_SkillBase::PlaySkill()
{	
	// 타게팅 시작지점을 CreateTask 세번째 인자로 넘겨준다.
	//ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	//FString StrEventTag(TEXT("Event.Skill"));
	//const FString StrParentTag(TEXT("Character.Skill"));
	//FGameplayEventData EventData;
	//
	//for (auto& Tag : AbilityTags)
	//{
	//	if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(*StrParentTag)) == true)
	//	{
	//		FString StrFullTag = Tag.ToString();
	//		StrFullTag = StrFullTag.Replace(*StrParentTag, TEXT(""));
	//		StrEventTag += StrFullTag;
	//		EventData.EventTag = FGameplayTag::RequestGameplayTag(*StrEventTag);
	//		EventData.Instigator = Character;
	//		EventData.InstigatorTags.AddTag(Tag);
	//		break;
	//	}
	//}
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Character, EventData.EventTag, EventData);

	m_PlaySkillTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlaySkill"), m_SkillMontage, 1.0f, m_SectionName);
	m_PlaySkillTask->OnCompleted.AddDynamic(this, &UGA_SkillBase::OnCompleteCallback);
	m_PlaySkillTask->OnInterrupted.AddDynamic(this, &UGA_SkillBase::OnInterruptedCallback);
	m_PlaySkillTask->ReadyForActivation();
}

void UGA_SkillBase::PlaySkill_TargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());

	// 타게팅 시스템에 등록. 
	UTargetSystemComponent* TargetSystemComponent = Character->FindComponentByClass<UTargetSystemComponent>();
	if(TargetSystemComponent != nullptr)
	{
		if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
		{
			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

			TargetSystemComponent->AddTarget(TargetSystemComponent->GetTargetActor(), HitResult.GetActor());
		}
		else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
		{
			for (auto& Target : TargetDataHandle.Data[0].Get()->GetActors())
			{
				TargetSystemComponent->AddTarget(TargetSystemComponent->GetTargetActor(), Target.Get());
			}
		}
		else
		{
			CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
			return;
		}
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	CalcMotionWarping(Character);

	// 여기에서 Indicator로 넘어가자.
	if (m_IndicatorClass != nullptr)
	{
		// Indicator Section
		m_Indicator = GetWorld()->SpawnActorDeferred<AIndicatorBase>(m_IndicatorClass, FTransform::Identity, Character, Character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FVector DesiredSize(1.0, 1.0, 1.0);
		// (임시) 상수로 표현중이지만 고쳐야한다. 0U은 Rectangle , 1U는 Circle로 표현할 예정.
		// (임시) 스킬의 모양은 스킬 어트리뷰트를 써야할지 지금처럼 그냥 멤버로 들고있어야할지 
		// 원하는 크기 절대값 (cm 단위)
		if (m_Indicator->GetIndicatorShape() == 0U)
		{
			DesiredSize.X = m_SkillData->Range;
			DesiredSize.Y = m_SkillData->Width;
			DesiredSize.Z = 1.0;
		}
		else
		{
			DesiredSize = DesiredSize * m_SkillData->Range;
			DesiredSize.Z = 1.0;
		}
		// 메시의 기본 Bounds 가져오기
		const double DefaultSize = 100.0;
		FVector ScaleFactor = DesiredSize / DefaultSize;
		m_Indicator->GetIndicatorMesh()->SetWorldScale3D(ScaleFactor);
		// SetLifeCycle
		m_Indicator->SetLifeCycle(m_SkillData->CastDelay);

		ATA_Skill* TargetActor = Cast<ATA_Skill>(m_TargetSystemComp->GetTargetActor());
		if (TargetActor != nullptr)
		{
			FRotator LookAtRotation = Character->GetActorRotation();
			// 마우스 위치로 방향을 계산
			if (TargetActor->GetConfirmationType() == EGameplayTargetingConfirmation::UserConfirmed)
			{
				FVector DestPos = UJWFunctionLibrary::ComputePositionFromMouse(Character);
				LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), DestPos);
			}

			// 캐릭터 위치와 회전은 유지
			FHitResult HitResult;
			FVector Start = Character->GetActorLocation() + FVector{0.f,0.f,50.f};
			// Rectangle 모양이면 위치를 보정
			EIndicatorShape Shape = static_cast<EIndicatorShape>(m_Indicator->GetIndicatorShape());
			if (Shape == EIndicatorShape::Rect)
			{
				Start += (LookAtRotation.Vector() * (m_Indicator->GetIndicatorMesh()->GetRelativeScale3D().X * m_SkillData->Range * 0.5f));	
			}
			FVector End = Start - FVector{ 0.f,0.f,1000.f };
			UJWFunctionLibrary::CheckCollisionTrace_LineSingleByChannel(Character, Start, End, ECC_Visibility, HitResult, FName(TEXT("FloorTrace")));
			const FVector SpawnLocation = HitResult.ImpactPoint + FVector{ 0.0f,0.0f,5.0f };

			// 앞에서 계산한 ScaleFactor 사용
			FTransform SpawnTransform(LookAtRotation, SpawnLocation, ScaleFactor);
			m_Indicator->FinishSpawning(SpawnTransform);

			m_IndicatorTask = UAbilityTask_WaitDelay::WaitDelay(this, m_SkillData->CastDelay);
			m_IndicatorTask->OnFinish.AddDynamic(this, &UGA_SkillBase::PlaySkill);
			m_IndicatorTask->ReadyForActivation();
		}
	}
	else
	{
		// Indicator가 없으면 바로 스킬 모션 재생
		PlaySkill();
	}
}

void UGA_SkillBase::CancelSkill_TargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UGA_SkillBase::CalcMotionWarping_Implementation(ACharacter* Character)
{
	/*
		이동 기능이 존재하는지로 구분
		자식에서 재정의해서 블루프린트에서 스킬별로 이동 기능을 각자 구현하도록 설계. ( 네이티브 코드단에서는 디폴트 구현만 한다. )
	*/
	UMotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UMotionWarpingComponent>();
	if(MotionWarpingComp != nullptr && m_SkillMontage->HasRootMotion() == true)
	{
		ATA_Base* TargetActor = Cast<ATA_Base>(m_TargetSystemComp->GetTargetActor());
		if (TargetActor != nullptr)
		{
			const FTransform& StartTransform = TargetActor->StartLocation.LiteralTransform;
			// 타게팅 시스템을 통해서 타겟을 바라보도록 모션워핑 설정
			if (m_HasMovement == true)
			{
				// Motion Warping 타겟 업데이트
				MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("TargetActor")), StartTransform.GetLocation(), StartTransform.GetRotation().Rotator());
			}
			else
			{
				// Motion Warping 타겟 업데이트
				MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("TargetActor")), Character->GetActorLocation(), StartTransform.GetRotation().Rotator());
			}
		}
	}
	else
	{
		if (m_TargetSystemComp != nullptr)
		{
			ATA_Base* TargetActor = Cast<ATA_Base>(m_TargetSystemComp->GetTargetActor());
			if (TargetActor != nullptr)
			{
				const FTransform& StartTransform = TargetActor->StartLocation.LiteralTransform;
				// 여기서 TargetActor를 세팅해 줘야한다.
				Character->SetActorRotation(StartTransform.GetRotation());
				Character->GetController()->SetControlRotation(StartTransform.GetRotation().Rotator());

				if (m_HasMovement == true)
				{
					Character->LaunchCharacter(StartTransform.GetRotation().GetForwardVector() * (Character->GetCharacterMovement()->MaxWalkSpeed * m_SkillSpeedRatio), true, true);
				}
			}
			else
			{
				// TargetActor가 없으면 실행 못한다. 
			}
		}
	}


}
