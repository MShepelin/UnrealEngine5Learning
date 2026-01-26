// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SDynamicBoxWidget.h"
#include "DynamicBoxWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDynamicBoxWidget : public UWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MovementBounds = FVector2D(300.f, 300.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D BoxSize = FVector2D(50.f, 50.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveSpeed = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RotationSpeed = 120.f;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
    TSharedPtr<SDynamicBoxWidget> SlateWidget;
};
