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
class ATA_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITYSYSTEMJW_API UTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetSystemComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	uint8 GetConfirmationType() const { return static_cast<uint8>(m_ConfirmationType); }

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor()const; 
	TArray<TWeakObjectPtr<AActor>>& GetTargetActors();
	FORCEINLINE bool IsTargeting() const { return m_IsTargeting; }
	void AddTargetActor(AActor* TargetActor);


	FTargetData& GetCurrentTargetData() { return CurrentTargetData; }
	TSubclassOf<ATA_Base> GetTargetActorClass() const { return m_TargetActorClass; }
	void SetCurrentTargetData(FTargetData InTargetData) { CurrentTargetData = InTargetData; }
	void SetTargetActorClass(TSubclassOf<ATA_Base> InTargetActorClass) { m_TargetActorClass = InTargetActorClass; }

	void AddTarget(AActor* TargetActor, AActor* Target);

	void StartTargeting();
	void EndTargeting();


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
	UPROPERTY(EditAnywhere, Category = "Property")
	float m_SearchRadius;
	float m_UpdatePeriod;

	FTargetData CurrentTargetData;		// 현재 타겟데이터 
	TEnumAsByte<EGameplayTargetingConfirmation::Type>  m_ConfirmationType;
	TSubclassOf<ATA_Base> m_TargetActorClass;

	TWeakObjectPtr<AActor> m_AutoTarget;
	TArray<TWeakObjectPtr<AActor>> m_TargetActors;
	TArray<FVector> m_TargetDestinations;
	FTimerHandle m_UpdateTimerHandle;

	uint8 m_IsTargeting : 1;
};
