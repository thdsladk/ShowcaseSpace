// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_DecideCommand.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Interface/BehaviorInterface.h"
#include "Header/GlobalEnum.h"

UBTTask_DecideCommand::UBTTask_DecideCommand()
{
	NodeName = TEXT("DecideCommand");

}

EBTNodeResult::Type UBTTask_DecideCommand::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 명령어가 없을 때만 여기로 들어온다.
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	//AMyAIController* AIController = CastChecked<AMyAIController>(OwnerComp.GetAIOwner());
	EBattleCommand CurrentCommand = EBattleCommand::Null;

	uint8 AggroGauge = BI->GetAggroGauge();
	if (AggroGauge == 100)
	{
		CurrentCommand = (EBattleCommand::SmashAttack);
	}
	else if(AggroGauge > 20)
	{
		int32 Random = FMath::RandRange(1, 10);
		if (Random <= 8)
		{
			CurrentCommand = (EBattleCommand::Attack);
		}
		else if(Random <=9)
		{
			CurrentCommand = (EBattleCommand::Attack);
			//CurrentCommand = (EBattleCommand::Defense);
		}
		else if (Random <= 10)
		{
			// (임시) 공격으로 대신 넣어 두었다 .
			CurrentCommand = (EBattleCommand::Attack);
			//CurrentCommand = (EBattleCommand::Cast);
		}
		
	}
	else if (AggroGauge == 0)
	{
		
	}
	
	// 명령 세팅을 해준다.
	BI->OnCommand(static_cast<uint8>(CurrentCommand));
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, static_cast<uint8>(CurrentCommand));

	return Result;
}


/*  기본 공격을 하다가 다른 공격을 섞어서 하자 . */
//EBattleCommand::Attack
//EBattleCommand::SmashAttack
//EBattleCommand::Defense
//EBattleCommand::Cast

//EBattleCommand::Shout
//EBattleCommand::FallBack
//EBattleCommand::Escape