// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapon/GASWeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "FunctionLibrary/JWFunctionLibrary.h"

// Sets default values
AGASWeaponBase::AGASWeaponBase() :
	 m_EquipmentType(EEquipmentType::None),
	 m_DebugThickness(1.0f),
	 m_DebugDuration(0.0f),
	 m_DebugCount(0U),
	 m_IsDebugMode(false),
	 m_IsAttacking(false)
{
 	// Tick 활성화: 디버그 선을 주기적으로 그리려면 true로 설정
	PrimaryActorTick.bCanEverTick = true;

	m_WeaponMesh_SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh_SM"));
	SetRootComponent(m_WeaponMesh_SM);

	m_WeaponMesh_SK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_SK"));
	m_WeaponMesh_SK->SetupAttachment(GetRootComponent());
	
	m_WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	m_WeaponCollisionBox->SetupAttachment(GetRootComponent());
	m_WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	m_WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGASWeaponBase::OnCollisionBoxBeginOverlap);
	m_WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &AGASWeaponBase::OnCollisionBoxEndOverlap);
}

void AGASWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr)
	{
		m_IsDebugMode = UJWFunctionLibrary::IsDebugMode(GetOwner());
	}
}

void AGASWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsDebugMode && m_WeaponCollisionBox && GetWorld() && m_IsAttacking)
	{
		// 월드 위치, 스케일된 박스 extents, 회전(quat)을 얻어 디버그 박스 그림
		const FVector Center = m_WeaponCollisionBox->GetComponentLocation();
		const FVector Extent = m_WeaponCollisionBox->GetScaledBoxExtent();
		const FQuat Rotation = m_WeaponCollisionBox->GetComponentQuat();
		if (m_DebugCount > 0U)
		{
			DrawDebugBox(GetWorld(), Center, Extent, Rotation, FLinearColor::Green.ToFColor(true), false, m_DebugDuration*30.f, 0, m_DebugThickness);
		}
		else
		{
			DrawDebugBox(GetWorld(), Center, Extent, Rotation, FLinearColor::Red.ToFColor(true), false, m_DebugDuration, 0, m_DebugThickness);
		}
	}
}

UPrimitiveComponent* AGASWeaponBase::GetWeaponCollisionBox()
{
	return m_WeaponCollisionBox;
}

void AGASWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instiagtor as the owning pawn for the weapon: %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
			OnTriggerHitTargetDynamic.Broadcast(OtherActor);
			m_DebugCount= FMath::Clamp(m_DebugCount+1U, 0U, 255U);
		}

		//TODO:Implement hit check for enemy characters
	}
}

void AGASWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instiagtor as the owning pawn for the weapon: %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
			m_DebugCount = FMath::Clamp(m_DebugCount - 1U, 0U, 255U);
		}

		//TODO:Implement hit check for enemy characters
	}
}



