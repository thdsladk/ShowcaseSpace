// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "GhostTrail.generated.h"

UCLASS()
class MYTEST_TOPDOWN_API AGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostTrail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void SetupGhostTrail(class ACharacter* pCharacter);

    UFUNCTION(BlueprintCallable, Category = "GhostTrail")
        void BeginGhostTrailEffect();

protected:

    UFUNCTION()
        void HandleTimelineUpdate(float Value);

    UFUNCTION()
        void HandleTimelineFinished();



protected:

    /** Ghost trail Material  */
    UPROPERTY(EditDefaultsOnly, Category = "Material")
    UMaterialInterface* m_GhostTrailMaterial;

    /** Ghost trail fade opacity parameter name  */
    UPROPERTY(EditDefaultsOnly, Category = "Opacity")
    FName m_ScalarParameterName = FName(TEXT("Opacity"));

    /** Ghost trail mesh opacity curve  */
    UPROPERTY(EditDefaultsOnly, Category = "Opacity")
    UCurveFloat* m_OpacityCurve;

    /** Ghost trail pose character  */
    UPROPERTY(BlueprintReadWrite, Category = "Character")
    TWeakObjectPtr<class ACharacter> m_CharacterRef;

    /** BeginPlay starts automatically  */
    UPROPERTY(EditDefaultsOnly, Category = "Trail Activation")
	bool m_bAutoActive = false;


    /** Ghost trail mesh  */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    class UPoseableMeshComponent* m_PoseableMeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
    USceneComponent* m_SceneRootComp;

    /** Ghost trail mesh opacity timeline  */
    //UPROPERTY()
    FTimeline m_OpacityTimeline;

    /** Ghost trail dynamic material  */
    UPROPERTY()
    UMaterialInstanceDynamic* m_DynamicMatInstance;


};
