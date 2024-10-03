// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "HealthRegenModifier.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHealthRegenModifier : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:

	UHealthRegenModifier();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition HealthPointsDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthPointsDef;
};
