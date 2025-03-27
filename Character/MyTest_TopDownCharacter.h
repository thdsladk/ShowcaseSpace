// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interface/InventoryInterface.h"

#include "MyTest_TopDownCharacter.generated.h"




UCLASS(Blueprintable)
class AMyTest_TopDownCharacter : public ACharacterBase, public IInventoryInterface
{
	GENERATED_BODY()

public:
	AMyTest_TopDownCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ChangeCameraView();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return m_CameraBoom; }

// Components Part
public:
	class UMyInventoryComponent* GetInventoryComponent() { return m_pInventoryComp; }
	class USkillComponent& GetSkillComponent() { return *m_pSkillComp; }


public:
	// Behavior Section
	virtual void Attack()override;
	virtual void AttackCheck()override;
	virtual void AttackEnd()override;
	virtual void OnHit()override;
	virtual void Death()override;


	// ArmCamera Section 
	void Look_UpDown(float Value);
	void Look_LeftRight(float Value);
	void Wheel_UpDown(float value);


	// Movement Function (override)
	virtual void MoveToFoward(float Value)override;
	virtual void MoveToRight(float Value)override;

	virtual	void Sprint()override;
	virtual	void SetSprint(float WalkSpeed , float CameraSpeed)override;
	virtual	void StopSprint()override;
	
	// HUD Section 
	UFUNCTION()
	void HUDUpdate(uint8 Type);

	// Animation Section
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnDeathMontageEnded();
	UFUNCTION()
	void OnHitMontageEnded();


	// Interact Section 
	void Click_F();
	void CheckForInteractable();

	virtual void UseItem(int32 Index)override;
	virtual void DropItem(int32 Index,FVector Pos = FVector(0.0,0.0,0.0))override;
	UFUNCTION()
		bool HasCurrentInteractable();
	UFUNCTION()
		void SetCurrentInteractable(class AMyInteractable* Interactable);

	
	// Damage Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool IsPlayerCharacter() override { return true; }

#pragma region Components Member

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* m_CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UMyInventoryComponent* m_pInventoryComp;
	UPROPERTY(VisibleAnywhere, Category = Component)
		class USkillComponent* m_pSkillComp;
	UPROPERTY(VisibleAnywhere, Category = Component)
		class UMyAnimInstance* m_pAnimInstance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = "true"))
		class UMyEquipmentComponent* m_pEquipmentComp;


#pragma endregion

protected:


	// Interactable Section
	UPROPERTY()
	float m_Reach =1000.f;
	UPROPERTY()
	TWeakObjectPtr<class AMyInteractable> m_CurrentInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString m_HelpText;

	UPROPERTY()
	bool m_bShoulderView = false;

	// Timer 
	FTimerHandle m_Timer_ItemCollision;

#pragma region DEBUG

private:
	FTimerHandle m_hDebug;
	void Debug();

#pragma endregion

};

