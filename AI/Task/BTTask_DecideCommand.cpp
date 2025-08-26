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
	// ��ɾ ���� ���� ����� ���´�.
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
			// (�ӽ�) �������� ��� �־� �ξ��� .
			CurrentCommand = (EBattleCommand::Attack);
			//CurrentCommand = (EBattleCommand::Cast);
		}
		
	}
	else if (AggroGauge == 0)
	{
		
	}
	
	// ��� ������ ���ش�.
	BI->OnCommand(static_cast<uint8>(CurrentCommand));
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyAIController::BattleCommandKey, static_cast<uint8>(CurrentCommand));

	return Result;
}


/*  �⺻ ������ �ϴٰ� �ٸ� ������ ��� ���� . */
//EBattleCommand::Attack
//EBattleCommand::SmashAttack
//EBattleCommand::Defense
//EBattleCommand::Cast

//EBattleCommand::Shout
//EBattleCommand::FallBack
//EBattleCommand::Escape