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
 * ����� �ƴ϶� �������̽��� ������  �ϸ� Ȱ��ȭ Flag�� ����� ������ ���� ��� ���� �־��ְų� �ؾ��� �� ����.
 * ���� AActor ��ü���� SetHidden�� �����Լ��� �Ǿ��־ �׳� ... UObject ��ü���� ����..
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
