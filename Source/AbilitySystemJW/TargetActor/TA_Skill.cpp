// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor/TA_Skill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "Attribute/CharacterSkillAttributeSet.h"
//#include "Attribute/WeaponAttributeSet.h"
#include "DrawDebugHelpers.h"
#include "Interface/CollisionTeamInterface.h"
#include "FunctionLibrary/JWFunctionLibrary.h"
#include "CharacterComponents/TargetSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"


#include "GameFramework/Character.h"

/*
	스킬 타겟액터 역할 : 
*/

ATA_Skill::ATA_Skill() :
	m_CollisionChannel(ECollisionChannel::ECC_Visibility),
	m_bIgnoreSelf(false)
{
}

void ATA_Skill::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (Character != nullptr)
	{
		UTargetSystemComponent* TSC = Character->GetComponentByClass<UTargetSystemComponent>();
		if (TSC != nullptr)
		{
			SkillTargetData = TSC->GetSkillTargetData();
		}
	}

	CalculateTargetAttributeType();
	CalculateStartLocation();
	CalculateTargetType();
}

void ATA_Skill::ConfirmTargeting()
{
	// 	외부에서 AbilitySystemComponent->LocalInputConfirm() 를 통해서 
	// 사용할려면 override로 기능을 재정의 해서 사용해야한다.  
	switch (m_ConfirmationType)
	{
		case EGameplayTargetingConfirmation::Instant : 
		{
			ConfirmTargetingAndContinue();
			if (bDestroyOnConfirmation == true)
			{
				Destroy();
			}
			break;
		}
		case EGameplayTargetingConfirmation::UserConfirmed :
		{
			ConfirmTargetingAndContinue();
			break;
		}

		case EGameplayTargetingConfirmation::Custom:
		{
			//ToDo 
			break;
		}
		case EGameplayTargetingConfirmation::CustomMulti:
		{
			//ToDo
			break;
		}
	}
}

void ATA_Skill::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();

	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (Character != nullptr)
	{
		UTargetSystemComponent* TSC = Character->GetComponentByClass<UTargetSystemComponent>();
		if (TSC != nullptr)
		{
			TSC->AddTargetActor(this);
		}
	}

	MakeCollisionShape();


	FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
	TargetDataReadyDelegate.Broadcast(DataHandle);

}

FGameplayAbilityTargetDataHandle ATA_Skill::MakeTargetData() const
{
	return FGameplayAbilityTargetDataHandle();
}

void ATA_Skill::MakeCollisionShape()
{
	SkillTargetData.CollisionShape.ShapeExtent = m_ShapeExtent;
	SkillTargetData.CollisionShape.MakeCollisionShape();
}

void ATA_Skill::CalculateTargetAttributeType()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (ASC != nullptr)
	{
		const UCharacterSkillAttributeSet* AttributeSet = ASC->GetSet<UCharacterSkillAttributeSet>();
		if (AttributeSet != nullptr)
		{
			m_Range = AttributeSet->GetSkillRange();
			//(임시) 반경으로 도형별로 크기를 정하기는 애매한거같고 모양별로 크기를 가지도록 하는게 더 좋을거같다.
			m_ShapeExtent.X = AttributeSet->GetSkillRadius();
			m_ShapeExtent.Y = AttributeSet->GetSkillRadius();
			m_ShapeExtent.Z = AttributeSet->GetSkillRadius();
		}	
	}
}

void ATA_Skill::CalculateStartLocation()
{
	if (SourceActor == nullptr)return;

	// 공통으로 마우스 위치를 바라보도록 설정.
	FVector DestLocation = UJWFunctionLibrary::ComputePositionFromMouse(SourceActor);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SourceActor->GetActorLocation(), DestLocation);

	switch (SkillTargetData.TargetStartPointType)
	{
		case ETargetStartPointType::Character:
		{
			this->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
			this->StartLocation.LiteralTransform = FTransform(LookAtRotation, SourceActor->GetActorLocation());
			break;
		}
		case ETargetStartPointType::WeaponSocket:
		{
			// (임시) 아직 보류 캐릭터 위치로 쓰고있자 : 웨폰이 두개일수도 있으니까 서브 웨폰 , 메인 웨폰을 나눠야 할수도 있고 아직 무기 체계가 정립되지 않았다. 
			this->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::SocketTransform;
			this->StartLocation.LiteralTransform = FTransform(LookAtRotation, SourceActor->GetActorLocation());
			break;
		}
		case ETargetStartPointType::Cursor:
		{
			// 캐릭터를 바라보도록 설정.
			this->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
			this->StartLocation.LiteralTransform = FTransform(LookAtRotation.GetInverse(), DestLocation);
			break;
		}
		case ETargetStartPointType::End:
		{
			break;
		}
	}
}

void ATA_Skill::CalculateTargetType()
{
	ICollisionTeamInterface* CTI = CastChecked<ICollisionTeamInterface>(SourceActor);
	if (CTI == nullptr) return;

	switch (SkillTargetData.TargetType)
	{
		case ETargetType::Self :
		{
			m_CollisionChannel = CTI->GetFriendlyCollisionTeambyChannel();
			m_bIgnoreSelf = false;
			break;
		}
		case ETargetType::Friendly:
		{
			m_CollisionChannel = CTI->GetFriendlyCollisionTeambyChannel();
			m_bIgnoreSelf = true;
			break;
		}
		case ETargetType::Hostile:
		{
			m_CollisionChannel = CTI->GetHostileCollisionTeambyChannel();
			m_bIgnoreSelf = true;
			break;
		}
		case ETargetType::Natural:
		{
			m_CollisionChannel = CTI->GetNeutralCollisionTeambyChannel();
			m_bIgnoreSelf = true;
			break;
		}
		default:
		{
			m_CollisionChannel = CTI->GetHostileCollisionTeambyChannel();
			m_bIgnoreSelf = true;
			break;
		}

	}
	
}
