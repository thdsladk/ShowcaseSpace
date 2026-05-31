// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectJWGameMode.h"
#include "UObject/ConstructorHelpers.h"

AProjectJWGameMode::AProjectJWGameMode()
	:m_IsDebugMode(true)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/JWProject/Blueprints/PlayerCharacter/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PCC(TEXT("/Game/JWProject/Blueprints/BP_MyPlayerController.BP_MyPlayerController_C"));
	if (PCC.Class != NULL)
	{
		PlayerControllerClass = PCC.Class;
	}

}

void AProjectJWGameMode::StartPlay()
{
	Super::StartPlay();	

}
