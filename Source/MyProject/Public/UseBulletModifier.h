// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UseBulletModifier.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UUseBulletModifier : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	
	UUseBulletModifier();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition BulletCountDef;
};

