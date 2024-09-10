// Fill out your copyright notice in the Description page of Project Settings.


#include "DecreaseHealthModifier.h"
#include "HealthAttributes.h"

UDecreaseHealthModifier::UDecreaseHealthModifier()
{
	HealthPointsDef.AttributeToCapture = UHealthAttributes::GetHealthPointsAttribute();
	HealthPointsDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthPointsDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(HealthPointsDef);
}

float UDecreaseHealthModifier::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	bool isHealthInfinite = false;
	GConfig->GetBool(TEXT("CustomVariables"), TEXT("InfiniteHealth"), isHealthInfinite, GGameIni);

	FAggregatorEvaluateParameters EvaluationParameters;

	float HealthPoints;
	GetCapturedAttributeMagnitude(HealthPointsDef, Spec, EvaluationParameters, HealthPoints);

	if (isHealthInfinite)
	{
		return 0;
	}
	
	// Ensure that we don't set health to a too low value
	constexpr float decreaseMagnitude = 2;
	return (HealthPoints < decreaseMagnitude) ? -HealthPoints : -decreaseMagnitude;
}
