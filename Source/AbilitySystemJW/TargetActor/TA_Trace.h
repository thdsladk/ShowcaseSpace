// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetActor/TA_Base.h"
#include "Header/CollisionStructTypes.h"
#include "Header/GlobalEnum.h"
#include "TA_Trace.generated.h"
/**
 *	StartTargeting : 타게팅 시작
 *	ConfirmTargetingAndContinue : 타게팅을 확정하고 이후 남은 프로세스 시작 
 *	ConfirmTargeting : 태스크 실행없이 타게팅만 확정 
 *	CancelTargeting : 타게팅 취소 
 * 
 * 
 *	타겟액터는 효과가 시작되는 지점과 방향을 이미 가지고 있다는 전제가 필요하다. 
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API ATA_Trace : public ATA_Base
{
	GENERATED_BODY()
public:
	ATA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;											
	virtual void ConfirmTargetingAndContinue() override;														
	//void SetShowDebug(bool InShowDebug) { m_bShowDebug = InShowDebug; } 
	//FORCEINLINE void ChangeShape(ECollisionShapeType CollisionShape) { m_CollisionShape = m_CollisionShape; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
	void MakeCollisionShape();

protected:

	UPROPERTY(EditAnywhere, Category = "Collision")
	FJWCollisionShape m_CollisionShape;

	UPROPERTY(EditAnywhere, Category = "AttributeType")
	EAttributeType m_TargetAttributeType;

protected:
	float m_Range;
	float m_Radius;
};
