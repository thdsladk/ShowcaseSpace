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
	// 명령어가 없을 때만 여기로 들어온다.
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	IBehaviorInterface* BI = CastChecked<IBehaviorInterface>(OwnerComp.GetAIOwner()->GetPawn());
	AMyAIController* AIController = CastChecked<AMyAIController>(OwnerComp.GetAIOwner());
	uint8 CurrentCommand = 0;

	if (AIController->UseCommandQueue(CurrentCommand) == false)
	{
		// 값이 없으면  큐를 채운다.
		for (int32 i = 0; i < 10; i++)
		{
			// 랜덤의 범위는 임시로 설정.
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
				// 명령 큐 세팅 실패. 
			}

		}

		if(AIController->UseCommandQueue(CurrentCommand) == false)
		{
			// 채웠는데도 값이 없다면 문제가 있다.
			return EBTNodeResult::Failed;
		}
	}
	// 명령 세팅을 해준다.
	BI->OnCommand(CurrentCommand);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("BattleCommand"), CurrentCommand);

	
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