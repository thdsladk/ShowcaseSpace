// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsMotionSystemComponent.generated.h"

class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITYSYSTEMJW_API UPhysicsMotionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhysicsMotionSystemComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Tick_Force();

public:
	void PlayKnockBack(const FVector& Direction, float Scale);
	void PlayKnockBack(const FVector& Force);

	void StartAddForce(const FVector& Force);
	void EndAddForce();


protected:
	UPROPERTY(EditAnywhere,Category = "Property")
	FVector m_Direction;
	UPROPERTY(EditAnywhere, Category = "Property")
	float m_Scale;

	UPROPERTY(EditAnywhere, Category = "Property")
	FVector m_Force;


	FTimerHandle m_ForceTimer;

};
