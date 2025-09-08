// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	RootComponent = m_Collision;
	m_Collision->SetSphereRadius(m_Radius);
	m_Collision->SetCollisionProfileName(TEXT("Projectile"));
	m_Collision->SetSimulatePhysics(true);
	m_Collision->SetNotifyRigidBodyCollision(true);

	m_Collision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	// 발사체 파티클 컴포넌트
	m_ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	m_ParticleComp->SetupAttachment(RootComponent);
	m_ParticleComp->bAutoActivate = true; // 스폰 시 자동 재생
	m_ParticleComp->SetUsingAbsoluteLocation(true);


}

void AProjectileBase::Init(FProjectileData& Data)
{
	// 런타임 데이터 세팅
	m_RuntimeData.ProjectileName = Data.ProjectileName;
	m_RuntimeData.ProjectileType = Data.ProjectileType;
	m_RuntimeData.Speed = Data.Speed;
	m_RuntimeData.GravityScale = Data.GravityScale;
	m_RuntimeData.Damage = Data.Damage;
	m_RuntimeData.DamageType = Data.DamageType;
	m_RuntimeData.LifeTime = Data.LifeTime;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_RuntimeData.Location = GetActorLocation();
	m_RuntimeData.Rotation = GetActorForwardVector();

	m_DestPos = m_RuntimeData.Location + (m_RuntimeData.Rotation*m_RuntimeData.Speed);
	

}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_ElapsedTime += DeltaTime;
	FMath::Clamp(m_ElapsedTime, 0.f, m_RuntimeData.LifeTime);
	double Time = m_ElapsedTime / m_RuntimeData.LifeTime;
	FVector Pos = FMath::Lerp(m_RuntimeData.Location, m_DestPos, Time);
	
	SetActorLocation(Pos);
	m_ParticleComp->SetWorldLocation(Pos);


	if(m_ElapsedTime >= m_RuntimeData.LifeTime)
	{
		Destroy();
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// TODO
		if (OtherActor && OtherActor != this && OtherComp)
		{
			// 폭발 파티클 스폰
			if (m_ExplosionParticle != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					m_ExplosionParticle,
					Hit.ImpactPoint,
					FRotator::ZeroRotator,
					true
				);
			}


			Destroy();
		}
	}
}

