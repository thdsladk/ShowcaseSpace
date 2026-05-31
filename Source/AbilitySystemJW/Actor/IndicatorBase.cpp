// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/IndicatorBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AIndicatorBase::AIndicatorBase()
    : m_LifeCycle(1.0f)
    , m_MaxGauge(0.2f)
    , m_MinGauge(0.0f)
    , m_Degree(0.0f)
    , m_GaugeGap(0.2f)
    , m_UpdateSecond(0.01f)
    , m_ElapsedTime(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // RootComponent 생성
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // StaticMeshComponent 생성 및 Root에 붙이기
    m_IndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IndicatorMesh"));
    m_IndicatorMesh->SetupAttachment(RootComponent);

    // 충돌 설정 (필요에 따라 변경)
    m_IndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 평면으로 설정.
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
    if (PlaneMesh.Succeeded())
    {
        m_IndicatorMesh->SetStaticMesh(PlaneMesh.Object);
    }

    // 원하는 절대 크기 (cm 단위)
    FVector DesiredSize(100.0f, 100.0f, 100.0f);

    // 메시의 기본 Bounds 가져오기
    FBoxSphereBounds Bounds = m_IndicatorMesh->GetStaticMesh()->GetBounds();
    FVector MeshSize = Bounds.BoxExtent * 2.0f; // BoxExtent는 반지름이므로 *2 해서 전체 길이

    // 스케일 비율 계산
    FVector ScaleFactor = DesiredSize / MeshSize;

    // 스케일 적용
    m_IndicatorMesh->SetWorldScale3D(ScaleFactor);

}

// Called when the game starts or when spawned
void AIndicatorBase::BeginPlay()
{
	Super::BeginPlay();
    m_DynMat = m_IndicatorMesh->CreateAndSetMaterialInstanceDynamic(0);
	
    GetWorld()->GetTimerManager().SetTimer(m_UpdateTimer, this,&AIndicatorBase::Update_MI, m_UpdateSecond, true);
}

void AIndicatorBase::SetGauge_MI(float MaxGauge,float MinGauge)
{
    //MaxGauge = FMath::Frac(MaxGauge);
    //MinGauge = FMath::Frac(MinGauge);


    m_MaxGauge = MaxGauge;

    if (m_IndicatorShape == EIndicatorShape::Rect)
    {
        
        m_DynMat->SetScalarParameterValue(FName(TEXT("Fill Amount")), m_MaxGauge);

    }
    else if(m_IndicatorShape == EIndicatorShape::Round)
    {
        m_DynMat->SetScalarParameterValue(FName(TEXT("Outer Radius")), m_MaxGauge);
        m_DynMat->SetScalarParameterValue(FName(TEXT("Inner Radius")), m_MinGauge);
    }

}

void AIndicatorBase::SetDegree_MI(float Degree)
{
    if (m_IndicatorShape == EIndicatorShape::Round)
    {
        m_Degree = Degree;
        
        m_DynMat->SetScalarParameterValue(FName(TEXT("Degree")), m_Degree);
    }

}

void AIndicatorBase::SetGaugeSpeed_MI(float UpdateSecond)
{
    m_UpdateSecond = UpdateSecond;
   
   FTimerManager& TimerManger =  GetWorld()->GetTimerManager();
   TimerManger.ClearTimer(m_UpdateTimer);
   TimerManger.SetTimer(m_UpdateTimer, this, &AIndicatorBase::Update_MI, m_UpdateSecond, true);
}

void AIndicatorBase::Update_MI()
{
    if (m_LifeCycle > m_ElapsedTime)
    {
        m_ElapsedTime += m_UpdateSecond;

        float Progress = FMath::Min(1.f, m_ElapsedTime / m_LifeCycle);

        if (m_IndicatorShape == EIndicatorShape::Rect)
        {
            m_MinGauge = Progress;
            m_MaxGauge = Progress;
        }
        else if (m_IndicatorShape == EIndicatorShape::Round)
        {
            m_MinGauge = Progress;
            m_MaxGauge = FMath::Min(1.f, m_MinGauge + m_GaugeGap);
        }

        SetGauge_MI(m_MaxGauge, m_MinGauge);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(m_UpdateTimer);

        m_LifeCycle = 1.0f;
        m_MaxGauge = 0.1f;
        m_MinGauge = 0.0f;
        m_Degree = 0.0f;
        m_UpdateSecond = 0.1f;

        Destroy();
    }
}


