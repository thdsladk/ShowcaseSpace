// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Header/DataStruct.h"
#include "ProjectileBase.generated.h"

class USphereComponent;

UCLASS(Blueprintable)
class MYTEST_TOPDOWN_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	void Init(struct FProjectileData& Data);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	/** 충돌 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* m_Collision;

	/** 발사체 비주얼 파티클 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* m_ParticleComp;

	/** 폭발 파티클 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* m_ExplosionParticle;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float m_Radius = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float m_ElapsedTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	FVector m_DestPos;




	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	FProjectileRunTimeData m_RuntimeData;


};
