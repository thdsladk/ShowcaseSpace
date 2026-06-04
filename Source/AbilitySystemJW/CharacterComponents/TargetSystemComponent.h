// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Header/CollisionStructTypes.h"
#include "Engine/OverlapResult.h"
#include "Header/GASStructTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "TargetSystemComponent.generated.h"

/*
* 보통은 타겟 액터가 1개 생성되지만, 여러개가 생성될 경우 0 인덱스의 타겟액터가 Default로 정의한다. 
*/
class AActor;
class ATA_Skill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITYSYSTEMJW_API UTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetSystemComponent();

public:	
	uint8 GetConfirmationType() const { return static_cast<uint8>(m_ConfirmationType); }

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor()const; 
	TArray<TWeakObjectPtr<AActor>>& GetTargetActors();
	FORCEINLINE bool IsTargeting() const { return m_IsTargeting; }
	void AddTargetActor(AActor* TargetActor);


	UFUNCTION(BlueprintCallable)
	AActor* GetTarget(int32 Index)const;
	UFUNCTION(BlueprintCallable)
	TArray<AActor*>  GetTargets(AActor* TargetActor)const;

	FSkillTargetData& GetSkillTargetData() { return SkillTargetData; }
	TSubclassOf<ATA_Skill> GetSkillTargetActorClass() const { return m_SkillTargetActorClass; }
	void SetSkillTargetData(FSkillTargetData InSkillTargetData) { SkillTargetData = InSkillTargetData; }
	void SetSkillTargetActorClass(TSubclassOf<ATA_Skill> InSkillTargetActorClass) { m_SkillTargetActorClass = InSkillTargetActorClass; }

	void AddTarget(AActor* TargetActor, AActor* Target);

	void StartTargeting();
	void EndTargeting();

#pragma region Calculation Function
public:
	bool Calculate_SingleLineTrace(const FVector Start, const FVector End, ECollisionChannel CollisionChannel, FHitResult& OutHitResult);
	bool Calculate_SingleTrace(FJWCollisionShape CollisionShape, const FVector Start, const FVector End, ECollisionChannel CollisionChannel, FHitResult& OutHitResult);
	bool Calculate_MultiTrace(FJWCollisionShape CollisionShape, const FVector Start, const FVector End, ECollisionChannel CollisionChannel, TArray<FHitResult>& OutHitResults);
	bool Calculate_Radius(FJWCollisionShape CollisionShape, const FVector Start,const float Radius, ECollisionChannel CollisionChannel, TArray<FOverlapResult>& OutOverlapResults);

#pragma endregion

#pragma region AutoTarget System
public:
	void EnableAutoTargetSystem();

	UFUNCTION(BlueprintCallable)
	AActor* GetAutoTarget() const;
protected:
	void UpdateAutoTargetSystem();
	bool AutoTargetOn();

#pragma endregion


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Property")
	float m_SearchRadius;
	float m_UpdatePeriod;

	FSkillTargetData SkillTargetData;		// 현재 타겟데이터 
	TEnumAsByte<EGameplayTargetingConfirmation::Type>  m_ConfirmationType;
	TSubclassOf<ATA_Skill> m_SkillTargetActorClass;

	TWeakObjectPtr<AActor> m_AutoTarget;
	TArray<TWeakObjectPtr<AActor>> m_TargetActors;
	TArray<TArray<TWeakObjectPtr<AActor>>> m_Targets;
	TArray<FVector> m_TargetDestinations;
	FTimerHandle m_UpdateTimerHandle;

	uint8 m_IsTargeting : 1;
};
