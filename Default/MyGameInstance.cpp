// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

constexpr int32 RandomPoolSize = 100;

UMyGameInstance::UMyGameInstance()
{
}


void UMyGameInstance::Init()
{
    Super::Init();

    m_RandomPool.Init(0, RandomPoolSize);

}

void UMyGameInstance::FillRandomPool()
{
    for (int32 i = 0; i < RandomPoolSize; i++)
    {
        m_RandomPool[i] = FMath::Rand();
    }
}

int32 UMyGameInstance::GetRandomRange(int32 Min, int32 Max)
{
    int32 Result = 0;
    if (m_RandomPool_Index >= m_RandomPool.Num())
    {
        FillRandomPool();
        m_RandomPool_Index = 0;   
    }

    Result = (m_RandomPool[m_RandomPool_Index] / (Max - Min + 1)) + Min;
    m_RandomPool_Index++;
    
    return Result;
}
