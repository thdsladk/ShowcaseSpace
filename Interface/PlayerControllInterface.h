// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerControllInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerControllInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTEST_TOPDOWN_API IPlayerControllInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ClickButton(uint8 Button) = 0;
	virtual void Click_F() = 0;
	virtual void Click_Q() = 0;
	virtual void Click_W() = 0;
	virtual void Click_E() = 0;
	virtual void Click_R() = 0;
	virtual void Click_V() = 0;
	virtual void Click_A() = 0;
	virtual void Click_S() = 0;
	virtual void Click_D() = 0;
	virtual void Click_G() = 0;
	virtual void Click_H() = 0;
	virtual void Click_J() = 0;
	virtual void Click_T() = 0;
	
	virtual void Click_Space() = 0;
	virtual void Click_Shift() = 0;
	virtual void ReleaseButton(uint8 Button) = 0;
	virtual void Release_Space() = 0;
	virtual void Release_Shift() = 0;
	virtual void ClickRMouse() = 0;

};
