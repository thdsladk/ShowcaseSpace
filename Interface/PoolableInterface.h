// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 상속이 아니라 인터페이스로 구현을  하면 활성화 Flag를 멤버로 강제할 수는 없어서 직접 넣어주거나 해야할 것 같다.
 * 보통 AActor 객체에는 SetHidden이 가상함수로 되어있어서 그냥 ... UObject 객체에서 쓰자..
 */
class MYTEST_TOPDOWN_API IPoolableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsUseableObject() = 0;
	virtual void ApplyHidden(bool bHide) = 0;
	virtual void AcquireObject() = 0;
	virtual void ReleaseObject() = 0;
};
