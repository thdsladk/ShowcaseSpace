// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckState.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"

#include "Interface/BehaviorInterface.h"


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

	
	uint8 State = Self->GetState();
	uint8 PreState = 0;
	uint8 Mode = Self->GetMode();
	//Debug
	if (State != PreState)
	{
		//UE_LOG(LogTemp, Log, TEXT("NPCState : %s"), *Str[State]);
		PreState = State;

		// 여기서 변경될때 행동을 넣어주자 . // 중간에 바뀌면 Cancel을 해줘야한다.


	}

	if (PreState == static_cast<uint8>(ENPCState::Die))
	{
		// 죽음 상태에서는 항상 die로만 귀결되도록 
		State = PreState;
	}


	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("State")), State);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("NPCMode")), Mode);



}
