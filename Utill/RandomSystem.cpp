// Fill out your copyright notice in the Description page of Project Settings.


#include "Utill/RandomSystem.h"

URandomSystem::URandomSystem()
{

}

int32 URandomSystem::PickRandom(int32 TotalSize)
{
	/*
		해당 원소 X 라고 두고  가중치 w 라고 두면 Xw : X의 가중치 w 를 의미 

		A ~ J까지의 원소를 두면 ,      < Aw * 누적 가중치  =  A의 확률 >
									  < Bw * (Aw ) = B의 확률 >
									  < Cw * (Aw+Bw) = C의 확률 > 
		
		
	*/
	if (m_List.IsEmpty() == false)
	{
		int32 Random = FMath::RandRange(1, TotalSize);

		int32 AccWeight = 0;
		for (auto Iter = m_List.begin(); Iter != m_List.end(); ++Iter)
		{
			AccWeight += static_cast<int32>((static_cast<double>(Iter.Value()) / m_TotalWeight) * static_cast<double>(TotalSize));
			if (Random <= AccWeight)
			{
				return Iter.Key();
			}
		}
		// 확률 계산 코드 에러 [ 코드 에러 범위 ] 
		return -2;
	}
	// 리스트에 원소가 없다.
	return -1;
}

void URandomSystem::AddElement(int32 Key, int32 Weight)
{
	if (m_List.Contains(Key) == true)
	{
		// 이미 값이 있으면 기존의 가중치를 지우고 새로운 가중치로 대체해서 총 가중치를 갱신
		m_TotalWeight -= static_cast<double>(m_List[Key]);
		
		m_List.Add(Key, Weight);

		// 총 가중치 동기화
		m_TotalWeight += static_cast<double>(Weight);
	}
	else
	{
		m_List.Add(Key, Weight);

		// 총 가중치 동기화
		m_TotalWeight += static_cast<double>(Weight);
	}
	
}

void URandomSystem::RemoveElement(int32 Key)
{
	if (m_List.Contains(Key) == true)
	{
		int32 Weight = m_List[Key];
		m_List.Remove(Key);

		// 총 가중치 동기화
		m_TotalWeight -= static_cast<double>(Weight);
	}
}
/// <summary>
///  기존 리스트가 있으면 합치는걸 진행 
///	 합칠 때는 키가 있으면 갱신하고 없으면 추가  
/// </summary>
/// <param name="List"></param>
void URandomSystem::AddList(TMap<int32,int32> List)
{
	
	if (List.IsEmpty() == false)
	{
		for (auto& Elem : List)
		{
			AddElement(Elem.Key, Elem.Value);
		}
	}

}
/// <summary>
/// 여러개의 키들을 지우는 기능
/// </summary>
/// <param name="List"></param>
void URandomSystem::RemoveList(TMap<int32,int32> List)
{
	if (List.IsEmpty() == false)
	{
		for (auto& Elem : List)
		{
			RemoveElement(Elem.Key);
		}
	}
}
