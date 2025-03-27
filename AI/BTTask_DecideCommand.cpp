// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DecideCommand.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Interface/BehaviorInterface.h"

UBTTask_DecideCommand::UBTTask_DecideCommand()
{
	NodeName = TEXT("DecideCommand");

}

EBTNodeResult::Type UBTTask_DecideCommand::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ��ɾ ���� ���� ����� ���´�.
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	AMyAIController* AIController = CastChecked<AMyAIController>(OwnerComp.GetAIOwner());
	uint8 CurrentCommand = 0;

	if (AIController->UseCommandQueue(CurrentCommand) == false)
	{
		// ���� ������  ť�� ä���.
		for (int32 i = 0; i < 10; i++)
		{
			// ������ ������ �ӽ÷� ����.
			int32 Random = FMath::RandRange(1, 8);

			switch (Random)
			{
			case (static_cast<uint8>(EBattleCommand::SmashAttack)):
			case (static_cast<uint8>(EBattleCommand::Defense)):
			{
				break;
			}
			default:
			{
				Random = static_cast<uint8>(EBattleCommand::Attack);
				break;
			}
			}

			if (AIController->AddCommandQueue(Random) == false)
			{
				// ��� ť ���� ����. 
			}

		}

		if(AIController->UseCommandQueue(CurrentCommand) == false)
		{
			// ä���µ��� ���� ���ٸ� ������ �ִ�.
			return EBTNodeResult::Failed;
		}
	}
	// ��� ������ ���ش�.
	BI->OnCommand(CurrentCommand);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), CurrentCommand);

	
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