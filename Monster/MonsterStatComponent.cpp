// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMonsterStatComponent::UMonsterStatComponent()
	
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
	m_BaseStat.Level = 1;
}


// Called when the game starts
void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMonsterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

