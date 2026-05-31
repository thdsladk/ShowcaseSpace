// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AOEBase.h"
#include "Interface/PlayerControllerInterface.h"

AAOEBase::AAOEBase()
	:m_PlayerController(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAOEBase::BeginPlay()
{
	Super::BeginPlay();
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn != nullptr)
	{
		AController* Controller = Pawn->GetController();
		if (Controller != nullptr)
		{
			m_PlayerController = CastChecked<APlayerController>(Controller);
			//IPlayerControllerInterface* PCI = CastChecked<IPlayerControllerInterface>(Controller);
			//m_ChangeMouseMoveHandle = PCI->OnChangeMouseMove().AddUObject(this, &AAOEBase::ChangeMouseMove);
		}
	}
	
}

void AAOEBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	IPlayerControllerInterface* PCI = Cast<IPlayerControllerInterface>(m_PlayerController);
	if (PCI != nullptr && PCI->OnChangeMouseMove().IsBoundToObject(this) == true)
	{
		PCI->OnChangeMouseMove().Remove(m_ChangeMouseMoveHandle);
	}
}

void AAOEBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_PlayerController != nullptr)
	{
		FHitResult MousePoint;
		bool bHit = m_PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MousePoint);
		if (bHit == true)
		{
			SetActorLocation(MousePoint.ImpactPoint);
		}

	}
	
}

void AAOEBase::ChangeMouseMove()
{
	if (m_PlayerController != nullptr)
	{
		FHitResult MousePoint;
		bool bHit = m_PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, MousePoint);
		if (bHit == true)
		{
			SetActorLocation(MousePoint.ImpactPoint);
		}
	}
}
