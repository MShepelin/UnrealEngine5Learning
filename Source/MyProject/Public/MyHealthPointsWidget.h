// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHealthPointsWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyHealthPointsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* AmmoCounter = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	class UImage* AmmoImage = nullptr;
};
