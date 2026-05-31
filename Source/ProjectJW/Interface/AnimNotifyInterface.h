// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimNotifyInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEndMove)
DECLARE_MULTICAST_DELEGATE(FOnEndSection)
DECLARE_MULTICAST_DELEGATE(FOnShootPoint)
DECLARE_MULTICAST_DELEGATE(FOnNotifyPoint)
DECLARE_MULTICAST_DELEGATE(FOnDeathPoint)






// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTJW_API IAnimNotifyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BindEndMove(FOnEndMove Delegate) = 0;
	virtual void BindEndSection(FOnEndSection Delegate) = 0;
	virtual void BindShootPoint(FOnShootPoint Delegate) = 0;
	virtual void BindNotifyPoint(FOnNotifyPoint Delegate) = 0;
	virtual FOnDeathPoint& GetDeathPoint() = 0;
};
