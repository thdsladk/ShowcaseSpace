// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyItem.h"
#include "MyEquipment.generated.h"

UCLASS()
class MYTEST_TOPDOWN_API AMyEquipment : public AMyItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEquipment();

	void SetEquipment(int32 NumEquipment);
	virtual void SetItem(int32 ID) override;

	void OnEquip();
	void DetachEquip();

	void Click_F();
	virtual void Interact_Implementation();
	virtual void OnPickedUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void OnCharacterBeginOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult
		)override;

	virtual void OnCharacterEndOverlap(
			UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;


	bool m_IsEquip;



};
