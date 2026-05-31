// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Physics/JWCollision.h"
#include "ProjectileData.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECTJW_API UProjectileData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UProjectileData();

public:

	// 데미지 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage;
	// 수명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float LifeSeconds;

	//// GameplayEffect for Hit Damage
	//UPROPERTY(EditAnywhere, Category = "Projectile|GameplayEffect")
	//TSubclassOf<UGameplayEffect> HitDamageEffect;

};
