// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_CheckState.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"

#include "Interface/BehaviorInterface.h"
#include "Header/BehaviorEnum.h"

UBTService_CheckState::UBTService_CheckState()
{
	NodeName = TEXT("SV_CheckState");
	Interval = 1.0f;
}


void UBTService_CheckState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	IBehaviorInterface* Self = Cast<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (Self == nullptr)
		return;
	/*
	FString Str[]
	{
		TEXT("Idle"),
		TEXT("Walk"),
		TEXT("Running"),
		TEXT("Chase"),
		TEXT("Attacking"),
		TEXT("Attacking_End"),
		TEXT("Hit"),
		TEXT("Defence"),
		TEXT("Stunned"),
		TEXT("Detect"),
		TEXT("Battle"),
		TEXT("Alert"),
		TEXT("Die"),
		TEXT("End")
	};
	*/
	
	uint8 State = static_cast<uint8>(Self->GetState());
	uint8 Mode = static_cast<uint8>(Self->GetMode());
	uint8 AggroGauge = Self->GetAggroGauge();

	//Debug
	if (State != m_PreState)
	{
		//UE_LOG(LogTemp, Log, TEXT("NPCState : %s"), *Str[State]);
		m_PreState = State;

		// 여기서 변경될때 행동을 넣어주자 . // 중간에 바뀌면 Cancel을 해줘야한다.


	}
	if (Mode != m_PreMode)
	{
		m_PreMode = Mode;
	}

	if (m_PreState == static_cast<uint8>(EBehaviorState::Die))
	{
		// 죽음 상태에서는 항상 die로만 귀결되도록 
		State = m_PreState;
	}

	// Sync Blackboard
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::StateKey, State);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::NPCModeKey, Mode);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyAIController::AggroGaugeKey, AggroGauge);
	
}
