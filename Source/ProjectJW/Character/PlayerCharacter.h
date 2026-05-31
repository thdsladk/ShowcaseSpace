// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"

class USpringArmComponent;


UCLASS(config=Game)
class PROJECTJW_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// To add mapping context
	virtual void BeginPlay()override;
	virtual void PostInitializeComponents() override;


#pragma region Camera Functions
public:
	/** Returns m_CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }
	/** Returns m_FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return m_FollowCamera; }
#pragma endregion

#pragma region Movement Functions
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
#pragma endregion

	// Character Control Section
public:
	virtual void SetCharacterControlData(const UCharacterControlData* CharacterControlData) override;

#pragma region Movement Interface
public:
	virtual void ShoulderMove(const FVector2D& Value)override;
	virtual void ShoulderLook(const FVector2D& Value)override;
	virtual void QuaterMove(FVector2D Value)override;
	virtual void WhealInput(const float& Value)override;

	virtual void RotateToInterp(FVector2D Value, float InterpSpeed = 720.f);
public:
	virtual void StartSprint()override;
	virtual void StopSprint()override;
	virtual void SetSprint(float Ratio)override;

public:
	virtual void InitCameraSpeed()override;
	virtual void SetCameraSpeed(float NewCameraSpeed)override;
	virtual void ScaleCameraSpeed(float Ratio)override;
#pragma endregion




//  구분선 ( 위는 함수 아래는 멤버 변수 ) 

#pragma region Components Memeber

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_FollowCamera;



#pragma endregion

protected:

	UPROPERTY(EditAnywhere, Category = "MouseWheal")
	float m_WhealScale;

	float m_OriginCameraLagSpeed;

};

