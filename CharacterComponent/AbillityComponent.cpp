// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/AbillityComponent.h"

// Sets default values for this component's properties
UAbillityComponent::UAbillityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbillityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAbillityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbillityComponent::UseAbillity(uint8 Btn)
{
}

void UAbillityComponent::CancelAbillity()
{
}

void UAbillityComponent::OnAbillityMontageEnded()
{
}

void UAbillityComponent::OnAbillityMontagePoint()
{
}

