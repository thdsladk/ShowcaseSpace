// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_ChangeCombatMode.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/PlayerControllerInterface.h"
#include "Interface/EquipmentInterface.h"
#include "Animation/AnimInstanceBase.h"
#include "AbilitySystemComponent.h"
#include "JWGameplayTag.h"


UGA_ChangeCombatMode::UGA_ChangeCombatMode()
	:m_CurrentSectionIndex(0U)
	, m_bEquip(true)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	m_NextSectionName[0] = FName("Draw");
	m_NextSectionName[1] = FName("Sheathe");


}

void UGA_ChangeCombatMode::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor);
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
	IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Character->GetController());
	
	// 트리거 이벤트 태그를 통해서 전투 모드 설정.
	m_CombatMode = m_CombatModeFromTriggerEventTag[TriggerEventData->EventTag];
	if (PCI->GetCombatMode() == uint8(m_CombatMode))
	{
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
		return;
	}


	FOnNotifyPoint Delegate;
	Delegate.AddUObject(this, &UGA_ChangeCombatMode::ChangeEquipment);
	AnimInstance->BindNotifyPoint(Delegate);

	UAbilityTask_PlayMontageAndWait* PlaySheatheTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ChangeCombatMode"), AnimInstance->GetCombatModeMontage(PCI->GetCombatMode()), 1.0f, GetNextSection());
	PlaySheatheTask->OnCompleted.AddDynamic(this, &UGA_ChangeCombatMode::OnCompleteCallback_Sheathe);
	PlaySheatheTask->OnInterrupted.AddDynamic(this, &UGA_ChangeCombatMode::OnInterruptedCallback_Sheathe);
	PlaySheatheTask->ReadyForActivation();


}

void UGA_ChangeCombatMode::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UGA_ChangeCombatMode::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


	m_CurrentSectionIndex = 0U;
}

void UGA_ChangeCombatMode::OnCompleteCallback_Sheathe()
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimInstanceBase* AnimInstance = CastChecked<UAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
	IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Character->GetController());

	//uint8 CombatMode = PCI->AddCombatMode();		// Add방식은 제거하고 그냥 Set으로 넣어준다.
	PCI->SetCombatMode(uint8(m_CombatMode));
	PCI->UpdateCharacterControl();
	
	// 게임 플레이 태그 등록 
	if (Character != nullptr)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
			
		for (auto& Tag : m_CombatModeTagList)
		{
			ASC->RemoveLooseGameplayTag(Tag);
		}
		ASC->AddLooseGameplayTag(m_CombatModeTagList[uint8(m_CombatMode)]);
		
		//FGameplayTagContainer CombatModeTags;
		//CombatModeTags.AddTag(JWTAG_PLAYER_IDLE);
		//CombatModeTags.AddTag(JWTAG_PLAYER_MELEE);
		//CombatModeTags.AddTag(JWTAG_PLAYER_RANGE);
		//CombatModeTags.AddTag(JWTAG_PLAYER_DUALBLADE);
		//
		//ASC->RemoveLooseGameplayTags(CombatModeTags);
		//
		//if (CombatMode == uint8(ECombatMode::IdleMode))
		//{
		//	ASC->AddLooseGameplayTag(JWTAG_PLAYER_IDLE);
		//}
		//else if (CombatMode == uint8(ECombatMode::MeleeMode))
		//{
		//	ASC->AddLooseGameplayTag(JWTAG_PLAYER_MELEE);
		//}
		//else if (CombatMode == uint8(ECombatMode::RangeMode))
		//{
		//	ASC->AddLooseGameplayTag(JWTAG_PLAYER_RANGE);
		//}
		//else if (CombatMode == uint8(ECombatMode::DualBladeMode))
		//{
		//	ASC->AddLooseGameplayTag(JWTAG_PLAYER_DUALBLADE);
		//}

	}

	UAbilityTask_PlayMontageAndWait* PlayDrawTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ChangeCombatMode"), AnimInstance->GetCombatModeMontage(m_CombatMode), 1.0f, GetNextSection());
	PlayDrawTask->OnCompleted.AddDynamic(this, &UGA_ChangeCombatMode::OnCompleteCallback_Draw);
	PlayDrawTask->OnInterrupted.AddDynamic(this, &UGA_ChangeCombatMode::OnInterruptedCallback_Draw);
	PlayDrawTask->ReadyForActivation();

}

void UGA_ChangeCombatMode::OnInterruptedCallback_Sheathe()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_ChangeCombatMode::OnCompleteCallback_Draw()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

void UGA_ChangeCombatMode::OnInterruptedCallback_Draw()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility);
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_ChangeCombatMode::ChangeEquipment()
{
	IEquipmentInterface* EI = CastChecked<IEquipmentInterface>(GetAvatarActorFromActorInfo());
	
	if (m_bEquip == true)
	{
		m_bEquip = false;
		EI->UnEquip(EEquipmentSocketName::WeaponSocketR);
		EI->UnEquip(EEquipmentSocketName::WeaponSocketL);
	}
	else
	{
		m_bEquip = true;
		EI->Equip(EEquipmentSocketName::WeaponSocketR);
		EI->Equip(EEquipmentSocketName::WeaponSocketL);
	}
}

FName UGA_ChangeCombatMode::GetNextSection()
{
	m_CurrentSectionIndex = (m_CurrentSectionIndex + 1) % 2;
	FName SectionName = m_NextSectionName[m_CurrentSectionIndex];

	return SectionName;
}
