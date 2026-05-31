// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TargetActor/TA_Weapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

#include "AbilitySystemJW.h"
#include "Attribute/CharacterAttributeSet.h"
#include "CharacterComponents/PawnCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Interface/CollisionTeamInterface.h"
#include "Interface/PawnCombatInterface.h"
#include "Items/Weapon/GASWeaponBase.h"
#include "Physics/JWCollision.h"
#include "Tag/JWGameplayTag.h"

#include "FunctionLibrary/JWFunctionLibrary.h"

/*
* 무기 오버랩을 이용한 타겟팅 방식.
* 하나의 오버랩만을 받아서 하나의 타겟 데이터를 전달하는 역할 
* [ 여러개의 오버랩으로 확장을 생각해서 TArray로 AActor를 두었다. ] 
*/

ATA_Weapon::ATA_Weapon()
{
}

void ATA_Weapon::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	if (SourceActor != nullptr)
	{
		// 아래 방식을 쓰는 이유는 특정 컴포넌트 타입에에 종속되지 않고 인터페이스를 통해서 내부에서 컴포넌트 변경이 이루어져도 작동에 문제가 없도록 하려고 
		//const UPawnCombatComponent* PawnCombatComp = SourceActor->GetComponentByClass<UPawnCombatComponent>();
		const UPawnCombatComponent* PawnCombatComp = CastChecked<IPawnCombatInterface>(SourceActor)->GetPawnCombatComponent();
		if (PawnCombatComp != nullptr)
		{
			for (auto Weapon : PawnCombatComp->GetCharacterAllCurrentEquippedWeapons())
			{
				if (Weapon->GetEquipmentType() == EEquipmentType::Shield) continue;

				m_OverlappedComponents.Add(Weapon->FindComponentByClass<UShapeComponent>());
				if (m_OverlappedComponents.Last() != nullptr)
				{
					m_OverlappedComponents.Last()->OnComponentBeginOverlap.AddDynamic(this, &ATA_Weapon::OnWeaponOverlapBegin);
				}
			}
		}

	}
	
}

void ATA_Weapon::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();

	for (auto Comp : m_OverlappedComponents)
	{	
		if (Comp != nullptr)
		{
			Comp->OnComponentBeginOverlap.RemoveDynamic(this, &ATA_Weapon::OnWeaponOverlapBegin);
		}
	}
}

void ATA_Weapon::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이제 이건(m_OverlappedActors) 타겟액터가 활성화 되어있는동안 충돌한 Actor를 수집하는 용도.
	m_OverlappedActors.AddUnique(OtherActor);
	

	if (SourceActor != nullptr)
	{
		bool HitDetected = false;
		FHitResult OutHitResult;

		// 각 캐릭터의 팀에 의해서 충돌 채널을 결정할 수 있도록 하자 
		// 결국 내부에서 충돌 함수를 한번더... 호출해주고 있다. 
		ICollisionTeamInterface* CTI = CastChecked<ICollisionTeamInterface>(SourceActor);
		HitDetected = UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(SourceActor, 0.0f, FCollisionShape::MakeSphere(100.0f), CTI->GetHostileCollisionTeambyChannel(), OutHitResult, FName("ATA_Weapon"));

		if (HitDetected == true)
		{
			FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData(OutHitResult);
			TargetDataReadyDelegate.Broadcast(DataHandle);
		}

	}

}

FGameplayAbilityTargetDataHandle ATA_Weapon::MakeTargetData(const FHitResult& HitResult) const
{
	FGameplayAbilityTargetDataHandle DataHandle;

	if (HitResult.GetActor() != nullptr)
	{
		// HitResult 기반 TargetData 생성
		FGameplayAbilityTargetData_SingleTargetHit* TargetData =
			new FGameplayAbilityTargetData_SingleTargetHit(HitResult);

		DataHandle.Add(TargetData);
	}

	return DataHandle;
}

