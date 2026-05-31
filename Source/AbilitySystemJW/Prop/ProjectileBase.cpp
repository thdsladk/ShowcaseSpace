// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/JWGameplayTag.h"
#include "Interface/CollisionTeamInterface.h"
#include "FunctionLibrary/JWFunctionLibrary.h"

// (임시) 어빌리티가 아니라서 ...
#include "Abilities/GameplayAbility.h"
// (임시)
#include "UI/DamageFontWidget.h"

#include "DrawDebugHelpers.h"
#include "Header/JWHeader.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // 충돌 캡슐
    m_CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCapsuleComp"));
    m_CollisionComp->InitCapsuleSize(5.f, 5.f * 4.f); // 반지름 5, 절반높이 20
    m_CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	m_CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlapBegin);
   
    RootComponent = m_CollisionComp;


    // 이동 컴포넌트
    m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    m_ProjectileMovement->UpdatedComponent = m_CollisionComp;
    m_ProjectileMovement->InitialSpeed = 3000.f;
    m_ProjectileMovement->MaxSpeed = 3000.f;
    m_ProjectileMovement->bRotationFollowsVelocity = true;
    m_ProjectileMovement->ProjectileGravityScale = 1.0f; // 중력 적용

	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComp"));
	m_MeshComp->SetupAttachment(RootComponent);
	m_MeshComp->SetRelativeRotation(FRotator(0.f, 0.f, 270.f));
	m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);



    InitialLifeSpan = m_LifeSeconds;


}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();


    
}

void AProjectileBase::FireInDirection(const FVector& ShootDirection, float PowerRatio)
{
    AddPower(PowerRatio);
    // 풀파워는 2배 이동속도로 가속되도록 한다. 
	FVector Velocity = ShootDirection * (m_ProjectileMovement->InitialSpeed + (m_ProjectileMovement->InitialSpeed * PowerRatio));
    m_ProjectileMovement->Velocity = Velocity;
    m_ProjectileMovement->Activate();

    m_CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
}

// 아직 원거리 공격 어빌리티에서 타격 판정을 시행하고 있지만 여기서는 이벤트만 발생시키고 
// GA_AttackHitCheck같은 어빌리티에서 근접 원격 공격 모두 판정하는 식으로 작성해도 좋을듯하다. 
// 모든 Hit에 대한 판정은 AttackHitCheck에서 할수 있도록 로직을 짜는것도 좋을거 같다. 
void AProjectileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp && OtherActor != GetOwner())
    {
        // 데미지 적용
        //UGameplayStatics::ApplyPointDamage(
        //    OtherActor,
        //    m_Damage,
        //    GetActorLocation(),
        //    Hit,
        //    GetInstigatorController(),
        //    this,
        //    nullptr
        //);


        UAbilitySystemComponent* SourceASC = CastChecked<IAbilitySystemInterface>(GetOwner())->GetAbilitySystemComponent();
        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

		FHitResult OutHitResult;
        bool HitDetected = false;
        ICollisionTeamInterface* CTI = CastChecked<ICollisionTeamInterface>(GetOwner());
        HitDetected = UJWFunctionLibrary::CheckCollisionTrace_SweepSingleByChannel(this, 0.0f, FCollisionShape::MakeSphere(100.0f), CTI->GetHostileCollisionTeambyChannel(), OutHitResult, FName("ProjectileCollision"));

        if (UJWFunctionLibrary::IsDebugMode(GetOwner()) == true)
        {
            UJWFunctionLibrary::Debug_ShapeSingleByChannel(this, 0.0f, FCollisionShape::MakeSphere(100.0f), HitDetected);
        }

        if (SourceASC != nullptr && TargetASC != nullptr && m_HitDamageEffect != nullptr && HitDetected == true)
        {

            // (임시) 임시로 이벤트를 발생시키는중 .   
            FGameplayEventData PayloadData;
            PayloadData.EventMagnitude = 1.0f;
            PayloadData.Instigator = SourceASC->GetAvatarActor();
            // (임시) LightHit만 임의로 부착해주었다.
            PayloadData.TargetTags.AddTag(JWTAG_EVENT_CHARACTER_LIGTHHIT);
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, JWTAG_SHARED_EVENT_HITREACT, PayloadData);


            // Damage Effect Part
            FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
            Context.AddHitResult(OutHitResult);
            Context.AddInstigator(SourceASC->GetAvatarActor(), GetOwner());

            FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(m_HitDamageEffect, 1.f, Context);
            if (SpecHandle.IsValid())
            {
                TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                Debug::Print(TEXT("Ranged Hit !!!"), FColor::Orange);
            }


        }
        else
        {
            Debug::Print(TEXT("Hit Fail !!!"), FColor::Red);
        }


        // 화살 연출
        m_ProjectileMovement->StopMovementImmediately();
        m_CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

        SetLifeSpan(3.f);
    }
}
