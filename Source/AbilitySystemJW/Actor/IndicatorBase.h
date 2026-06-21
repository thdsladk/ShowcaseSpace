// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "IndicatorBase.generated.h"

UENUM()
enum class EIndicatorShape : uint8
{
	Rect UMETA(DisplayName = "Rect"),
	Round UMETA(DisplayName = "Round"),
	End
};

UCLASS()
class ABILITYSYSTEMJW_API AIndicatorBase : public ADecalActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	AIndicatorBase();

public:
	FORCEINLINE uint8 GetIndicatorShape() const { return uint8(m_IndicatorShape); }
	FORCEINLINE void SetLifeCycle(float LifeCycle) { m_LifeCycle = LifeCycle; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Material Functions

public:
	void SetGauge_MI(float MaxGauge,float MinGauge = 0.f);
	void AddGauge_MI(float MaxGauge,float MinGauge = 0.f) { SetGauge_MI(m_MaxGauge + MaxGauge,m_MinGauge + MinGauge); }
	void SetDegree_MI(float Degree);
	void SetGaugeSpeed_MI(float UpdateSecond);

	void Update_MI();

#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> m_IndicatorMesh;

	TObjectPtr<UMaterialInstanceDynamic> m_DynMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float m_LifeCycle;

#pragma region Material Members
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	float m_MaxGauge;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	float m_MinGauge;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	float m_Degree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	float m_GaugeGap;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	float m_UpdateSecond;

	float m_ElapsedTime;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	EIndicatorShape m_IndicatorShape;

private:
	FTimerHandle m_UpdateTimer;

#pragma endregion
};



