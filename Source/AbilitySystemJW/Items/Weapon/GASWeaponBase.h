// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Header/GlobalEnum.h"
#include "GASWeaponBase.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnWeaponHitTarget, AActor*);
DECLARE_DELEGATE_OneParam(FOnWeaponPulledFromTarget, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerHitTargetDynamic, AActor*, HitActor);

UCLASS()
class ABILITYSYSTEMJW_API AGASWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGASWeaponBase();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE EEquipmentType GetEquipmentType() const{ return m_EquipmentType; }
	UPrimitiveComponent* GetWeaponCollisionBox();
	FORCEINLINE void AttackOn() { m_IsAttacking = true; }
	FORCEINLINE void AttackOff() { m_IsAttacking = false; }

protected:
	UFUNCTION()
	void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FOnWeaponHitTarget OnWeaponHitTarget;
	FOnWeaponPulledFromTarget OnWeaponPulledFromTarget;
	FOnTriggerHitTargetDynamic OnTriggerHitTargetDynamic;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UStaticMeshComponent* m_WeaponMesh_SM;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* m_WeaponMesh_SK;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UBoxComponent* m_WeaponCollisionBox;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	EEquipmentType m_EquipmentType;

	// Debug
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (ClampMin = "0.0"))
	float m_DebugThickness;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (ClampMin = "0.0"))
	float m_DebugDuration;


private:
	uint8 m_DebugCount;
	uint8 m_IsDebugMode : 1;
	uint8 m_IsAttacking : 1;

};
