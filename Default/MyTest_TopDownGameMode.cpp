// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest_TopDownGameMode.h"
#include "MyTest_TopDownPlayerController.h"
#include "MyTest_TopDownCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


AMyTest_TopDownGameMode::AMyTest_TopDownGameMode()
	:m_ClearScore(3),m_CurrentScore(0),m_bIsCleared(false)
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyTest_TopDownPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

}

void AMyTest_TopDownGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	m_CurrentScore = NewPlayerScore;

	// 싱글이라서 모든 플레이어컨트롤러를 가져오지 않고 첫번째만 가져온다. 
	AMyTest_TopDownPlayerController* PlayerController = Cast<AMyTest_TopDownPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController != nullptr)
	{
		PlayerController->GameScoreChanged(m_CurrentScore);
	}

	if (m_CurrentScore >= m_ClearScore)
	{
		m_bIsCleared = true;

		if (PlayerController)
		{
			PlayerController->GameClear();
		}

	}


}

void AMyTest_TopDownGameMode::OnPlayerDead()
{
	AMyTest_TopDownPlayerController* PlayerController = Cast<AMyTest_TopDownPlayerController>(GetWorld()->GetFirstLocalPlayerFromController());
	if (PlayerController != nullptr)
	{
		PlayerController->GameOver();
	}
}
