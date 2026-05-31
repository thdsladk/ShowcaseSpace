// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 위젯에 관련된 모든 기능들을 정의해서 쓰기 위함
 * 캐릭터 게이지 / 궁수 게이지 / 데미지 폰트 / 상태 표시창 등등  
 */
class ABILITYSYSTEMJW_API IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget")
	void SetRangeGaugeBar(bool bVisible);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget")
	void SetVisibleGaugeBar(bool bVisible);

};
