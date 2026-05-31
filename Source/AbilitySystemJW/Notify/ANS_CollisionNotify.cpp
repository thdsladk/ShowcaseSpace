// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ANS_CollisionNotify.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "CharacterComponents/PawnCombatComponent.h"
#include "Interface/PawnCombatInterface.h"
/*
* 여기서 CastChecked로 확인을 해서 터트리면 OwnerActor가 Pawn이 아닌 경우가 있을수 있다. 
* [ 애니메이션 에디터 객체가 몽타주에서는 OwnerActor로 들어온다 . ]
*/
UANS_CollisionNotify::UANS_CollisionNotify()
	: m_ComboAttackLevel(1.0f)
{
}

void UANS_CollisionNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor->IsA(APawn::StaticClass()) == false) 
		return;
	UPawnCombatComponent* PawnCombatComp = Cast<IPawnCombatInterface>(OwnerActor)->GetPawnCombatComponent();
	
	if (PawnCombatComp != nullptr)
	{
		PawnCombatComp->ToggleWeaponCollision(true);

		// 공격 신호를 보내는 이벤트
		if (OwnerActor != nullptr)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = m_ComboAttackLevel;
			PayloadData.Instigator = OwnerActor;
			PayloadData.TargetTags.AddTag(m_InflictedStatusTag);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, m_TriggerGameplayTag, PayloadData);
		}
	}
}

void UANS_CollisionNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation,EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor->IsA(APawn::StaticClass()) == false)
		return;
	UPawnCombatComponent* PawnCombatComp = CastChecked<IPawnCombatInterface>(OwnerActor)->GetPawnCombatComponent();

	if (PawnCombatComp != nullptr)
	{
		PawnCombatComp->ToggleWeaponCollision(false);

		// 종료 신호를 보내는 이벤트
		if (OwnerActor != nullptr)
		{
			FGameplayEventData PayloadData;
			//PayloadData.EventMagnitude = m_ComboAttackLevel;
			//PayloadData.Instigator = OwnerActor;
			//PayloadData.TargetTags.AddTag(m_InflictedStatusTag);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, m_EndGameplayTag, PayloadData);
		}
	}
}
