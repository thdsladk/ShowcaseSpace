// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostTrail.h"


#include "GameFramework/Character.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"



// Sets default values
AGhostTrail::AGhostTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_bAutoActive = true;

	m_SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = m_SceneRootComp;

	m_PoseableMeshComp = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMeshComp"));
	m_PoseableMeshComp->SetupAttachment(RootComponent);



    static ConstructorHelpers::FObjectFinder<UMaterialInterface> GhostTrail(TEXT("/Script/Engine.Material'/Game/TopDown/GhostTrail/MT_GhostTrail.MT_GhostTrail'"));
    if (GhostTrail.Succeeded())
    {
        m_GhostTrailMaterial = GhostTrail.Object;
    }

    static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloat(TEXT("/Script/Engine.CurveFloat'/Game/TopDown/GhostTrail/CF_GhostTrail.CF_GhostTrail'"));
    if (CurveFloat.Succeeded())
    {
        m_OpacityCurve = CurveFloat.Object;
    }


    
}

// Called when the game starts or when spawned
void AGhostTrail::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_bAutoActive)
		BeginGhostTrailEffect();

}

// Called every frame
void AGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_OpacityTimeline.TickTimeline(DeltaTime);
}

void AGhostTrail::SetupGhostTrail(ACharacter* pCharacter)
{
    if(nullptr != pCharacter)
        m_CharacterRef = pCharacter;
}

void AGhostTrail::BeginGhostTrailEffect()
{
    if (m_OpacityCurve && m_CharacterRef.IsValid() && m_GhostTrailMaterial)

    {

        m_PoseableMeshComp->SetSkeletalMesh(m_CharacterRef->GetMesh()->SkeletalMesh);
        m_PoseableMeshComp->CopyPoseFromSkeletalComponent(m_CharacterRef->GetMesh());
        m_PoseableMeshComp->SetWorldScale3D(FVector(1.2f, 1.2f, 1.2f));


        m_DynamicMatInstance = UMaterialInstanceDynamic::Create(m_GhostTrailMaterial, this);
        m_DynamicMatInstance->SetScalarParameterValue(m_ScalarParameterName, 0.1f);


        TArray <FSkeletalMaterial> materials = m_CharacterRef->GetMesh()->SkeletalMesh->Materials;

        for (int32 Index = 0; Index != materials.Num(); ++Index)
        {
            m_PoseableMeshComp->SetMaterial(Index, m_DynamicMatInstance);
        }

        FOnTimelineFloat TimelineUpdateCallback;
        FOnTimelineEventStatic TimelineFinishedCallback;


        TimelineUpdateCallback.BindUFunction(this, FName{ TEXT("HandleTimelineUpdate" }));
        TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("HandleTimelineFinished") });


        m_OpacityTimeline.AddInterpFloat(m_OpacityCurve, TimelineUpdateCallback);
        m_OpacityTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
        m_OpacityTimeline.SetTimelineLength(0.5f);
        m_OpacityTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
        m_OpacityTimeline.SetLooping(false);
        m_OpacityTimeline.PlayFromStart();
    }


}

void AGhostTrail::HandleTimelineUpdate(float Value)
{
    m_DynamicMatInstance->SetScalarParameterValue(m_ScalarParameterName, Value);
}

void AGhostTrail::HandleTimelineFinished()
{
    Destroy();
}

