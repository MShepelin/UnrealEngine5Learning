// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegenModifier.h"
#include "HealthAttributes.h"

UHealthRegenModifier::UHealthRegenModifier()
{
	HealthPointsDef.AttributeToCapture = UHealthAttributes::GetHealthPointsAttribute();
	HealthPointsDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthPointsDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(HealthPointsDef);

	MaxHealthPointsDef.AttributeToCapture = UHealthAttributes::GetMaxHealthPointsAttribute();
	MaxHealthPointsDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthPointsDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxHealthPointsDef);
}

float UHealthRegenModifier::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;

	float HealthPoints;
	GetCapturedAttributeMagnitude(HealthPointsDef, Spec, EvaluationParameters, HealthPoints);

	float MaxHealthPoints;
	GetCapturedAttributeMagnitude(MaxHealthPointsDef, Spec, EvaluationParameters, MaxHealthPoints);

	return (HealthPoints < MaxHealthPoints) ? 1.f : 0.f;
}
