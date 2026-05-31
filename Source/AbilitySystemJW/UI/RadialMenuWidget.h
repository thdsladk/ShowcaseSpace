// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "RadialMenuWidget.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEMJW_API URadialMenuWidget : public UGASUserWidget
{
	GENERATED_BODY()

public:
	URadialMenuWidget();


    UFUNCTION(BlueprintCallable, Category = "RadialMenu")
    void PlayRadialMenu();
    UFUNCTION(BlueprintCallable, Category = "RadialMenu")
	void EndRadialMenu();
    
#pragma region GetSet Function

public:
    UFUNCTION(BlueprintCallable,Category = "RadialMenu")
    FORCEINLINE bool IsActive() const { return m_IsActive; }
    UFUNCTION(BlueprintCallable, Category = "RadialMenu")
    FORCEINLINE bool IsOpening()const { return m_IsOpening; }
    UFUNCTION(BlueprintCallable, Category = "RadialMenu")
    FORCEINLINE bool IsClosing()const { return m_IsClosing; }

#pragma endregion

protected:
    void MenuTick();
	void OpeningTick();
	void ClosingTick();
protected:
    // 에디터의 CanvasPanel과 이름을 맞춰야 합니다.
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MainPanel;

    // 슬롯에 생성할 버튼 위젯 클래스 (에디터에서 할당)
    UPROPERTY(EditAnywhere, Category = "RadialMenu")
    TSubclassOf<UUserWidget> SlotWidgetClass;

    // 반지름 설정
    UPROPERTY(EditAnywhere, Category = "RadialMenu|Value")
    float m_Radius = 50.f;

    UPROPERTY(EditAnywhere, Category = "RadialMenu|Value")
    float m_TickInterval = 0.01f;

    UPROPERTY(EditAnywhere, Category = "RadialMenu|Value")
    float m_PlaySpeed = 1.0f;

    UPROPERTY(EditAnywhere, Category = "RadialMenu|Curve")

	TObjectPtr<UCurveTable> m_CurveTable;

    
private:
    TArray<FVector2D> m_TargetPositions;
    float m_CurrentPercent = 0.f; // 0.0f~ 1.0f
    uint8 m_IsActive : 1;
    uint8 m_IsOpening : 1;
    uint8 m_IsClosing : 1;

    FTimerHandle m_MenuTickHandle;



    

};
