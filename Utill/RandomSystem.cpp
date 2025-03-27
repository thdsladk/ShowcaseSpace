// Fill out your copyright notice in the Description page of Project Settings.


#include "Utill/RandomSystem.h"

URandomSystem::URandomSystem()
{

}

int32 URandomSystem::PickRandom(int32 TotalSize)
{
	/*
		�ش� ���� X ��� �ΰ�  ����ġ w ��� �θ� Xw : X�� ����ġ w �� �ǹ� 

		A ~ J������ ���Ҹ� �θ� ,      < Aw * ���� ����ġ  =  A�� Ȯ�� >
									  < Bw * (Aw ) = B�� Ȯ�� >
									  < Cw * (Aw+Bw) = C�� Ȯ�� > 
		
		
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
		// Ȯ�� ��� �ڵ� ���� [ �ڵ� ���� ���� ] 
		return -2;
	}
	// ����Ʈ�� ���Ұ� ����.
	return -1;
}

void URandomSystem::AddElement(int32 Key, int32 Weight)
{
	if (m_List.Contains(Key) == true)
	{
		// �̹� ���� ������ ������ ����ġ�� ����� ���ο� ����ġ�� ��ü�ؼ� �� ����ġ�� ����
		m_TotalWeight -= static_cast<double>(m_List[Key]);
		
		m_List.Add(Key, Weight);

		// �� ����ġ ����ȭ
		m_TotalWeight += static_cast<double>(Weight);
	}
	else
	{
		m_List.Add(Key, Weight);

		// �� ����ġ ����ȭ
		m_TotalWeight += static_cast<double>(Weight);
	}
	
}

void URandomSystem::RemoveElement(int32 Key)
{
	if (m_List.Contains(Key) == true)
	{
		int32 Weight = m_List[Key];
		m_List.Remove(Key);

		// �� ����ġ ����ȭ
		m_TotalWeight -= static_cast<double>(Weight);
	}
}
/// <summary>
///  ���� ����Ʈ�� ������ ��ġ�°� ���� 
///	 ��ĥ ���� Ű�� ������ �����ϰ� ������ �߰�  
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
/// �������� Ű���� ����� ���
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
