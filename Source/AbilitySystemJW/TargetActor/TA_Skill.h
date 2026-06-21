// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetActor/TA_Base.h"
#include "Header/GlobalEnum.h"
#include "Header/GASStructTypes.h"
#include "TA_Skill.generated.h"

/**
 *   타게팅을 할때 타겟액터에게 필요한 변수들 
 *	1. 시작 위치 : 어느 지점에서 타게팅을 시작할지 ( 캐릭터 위치 , 소캣 위치 , 마우스 위치 등등 ) 
 *	2. 시작 방향 : 어느 방향으로 타게팅을 시작할지 ( 캐릭터 방향 , 소캣 방향 , 캐릭터에서 마우스 위치를 통한 바라보는 방향 ) 
 *	3. 충돌 채널 : 타게팅을 할 채널( 적,아군,월드 등등) 선택
 *  4. 거리 : 타게팅의 최대 범위 
 *  5. 반경 : 타게팅의 범위 ( 선형 , 원형 , 사각형 등등 모양과 크기 둘다 있으면 좋다 . ) 
 *  
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_Skill : public ATA_Base
{
	GENERATED_BODY()
public:
	ATA_Skill();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargeting() override;
	virtual void ConfirmTargetingAndContinue() override;
	//void SetShowDebug(bool InShowDebug) { m_bShowDebug = InShowDebug; } 

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
	void MakeCollisionShape();

protected:
	void CalculateTargetAttributeType();
	void CalculateStartLocation();
	void CalculateTargetType();

protected:
	FSkillTargetData SkillTargetData;
	ECollisionChannel m_CollisionChannel;
	uint8 m_bIgnoreSelf : 1;

protected:
	float m_Range;
	//float m_Radius;
	FVector m_ShapeExtent;
	bool m_SuccessSkill = true;
};
