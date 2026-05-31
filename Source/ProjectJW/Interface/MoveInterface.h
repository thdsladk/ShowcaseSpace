// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MoveInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMoveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTJW_API IMoveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ShoulderMove(const FVector2D& Value) = 0;
	virtual void ShoulderLook(const FVector2D& Value) = 0;
	virtual void QuaterMove(FVector2D Value) = 0;
	virtual void WhealInput(const float& Value) = 0;

	virtual void ReleaseMove(const FVector2D Value) = 0;

	virtual void RotateToInterp(FVector2D Value, float InterpSpeed = 720.f) = 0;

	virtual void StartSprint()=0;
	virtual void StopSprint()=0;
	virtual void SetSprint(float Ratio) = 0;

	virtual void InitCameraSpeed() = 0;
	virtual void SetCameraSpeed(float NewCameraSpeed) = 0;
	virtual void ScaleCameraSpeed(float Scale) = 0;
};
