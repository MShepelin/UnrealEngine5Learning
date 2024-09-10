// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DecreaseHealthModifier.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDecreaseHealthModifier : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UDecreaseHealthModifier();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition HealthPointsDef;
};
