// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthPointsWidget.h"
#include "Components/TextBlock.h"

void UMyHealthPointsWidget::NativeConstruct()
{
	AmmoCounter->SetText(FText::FromString(TEXT("Ammo")));
}
