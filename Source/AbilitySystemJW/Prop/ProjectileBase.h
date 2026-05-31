// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/JWCollision.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;
class UGameplayEffect;
class UUserWidget;

UCLASS(Blueprintable)
class ABILITYSYSTEMJW_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	//virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FORCEINLINE void SetPower(float Power) { m_Damage = Power; }
	FORCEINLINE void AddPower(float PowerRadio) { m_Damage += (m_Damage*PowerRadio); }

public:
	// 발사 기능
	void FireInDirection(const FVector& ShootDirection,float Power = 0.f);

private:
	// 충돌 처리 함수
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> m_ProjectileMovement;

	// 충돌 감지용 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> m_CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> m_MeshComp;

	// 이동 처리

	// 데미지 값 (Data Asset에서 세팅 가능)
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float m_Damage = 20.f;

	// 수명
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float m_LifeSeconds = 10.f;

	// GameplayEffect for Hit Damage
	UPROPERTY(EditAnywhere, Category = "Projectile|GameplayEffect")
	TSubclassOf<UGameplayEffect> m_HitDamageEffect;

	// Damage Font
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> m_DamageFontClass;
	

};
