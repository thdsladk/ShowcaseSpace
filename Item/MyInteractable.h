// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractable.generated.h"



UCLASS()
class MYTEST_TOPDOWN_API AMyInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyInteractable();

	FORCEINLINE FString GetHelpText() { return m_InteractableHelpText; }
	FORCEINLINE bool GetInCharacter() { return m_bInCharacter; }
	FORCEINLINE void SwitchVisiable() { m_bVisiable = !m_bVisiable; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnCharacterBeginOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult
		);

	UFUNCTION()
	virtual void OnCharacterEndOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	virtual void Init();
	void Click_F();
	virtual void Interact_Implementation();
	virtual int32 UsingItem() { return 0; }
	virtual void Replace(FVector Pos);
	virtual void SetHidden(bool bHide);


protected:
	UPROPERTY(EditAnywhere, Category = Info, meta = (AllowPrivateAccess = true))
	FString m_InteractableHelpText;


	UPROPERTY(VisibleAnywhere, Category = Interactable)
	TObjectPtr<class UBoxComponent> m_Trigger;
	UPROPERTY(VisibleAnywhere, Category = HelpTip)
	TObjectPtr<class UWidgetComponent> m_HelpTextComp;
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> m_MeshComp;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UNiagaraComponent> m_Effect;

	bool m_bVisiable;
	bool m_bInCharacter;



};
