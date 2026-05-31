// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LinkedActionComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITYSYSTEMJW_API ULinkedActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULinkedActionComponent();

	FORCEINLINE int32 PrevAction()const { return ( (m_InputIDs.IsValidIndex(0) == true) ? m_InputIDs[0] : -1);   }
	FORCEINLINE int32 NumActions()const { return m_InputIDs.Num(); }
	void AddAction(int32 InputID);
	int32 PopAction();
	FORCEINLINE void ClearActions() { m_InputIDs.Empty(); }

	void EndAction(int32 InputID);
	void ExcuteAction();






protected:
	TArray<int32> m_InputIDs;
	
	uint8 m_IsActiveLinkedAction : 1;

};
