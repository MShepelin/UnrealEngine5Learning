// Fill out your copyright notice in the Description page of Project Settings.


#include "UseBulletModifier.h"
#include "AmmoAttributes.h"

namespace MyVariables
{
	bool AllowInfiniteAmmoForGAS = false;
	FAutoConsoleVariableRef AllowInfiniteAmmoForGASConsoleRef(
		TEXT("cvar.infi_ammo"),
		AllowInfiniteAmmoForGAS,
		TEXT("Allows infinite ammo for everyone using ammo component"),
		ECVF_Default
	);
};

UUseBulletModifier::UUseBulletModifier()
{
	BulletCountDef.AttributeToCapture = UAmmoAttributes::GetBulletCountAttribute();
	BulletCountDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BulletCountDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(BulletCountDef);
}

float UUseBulletModifier::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	bool isHealthInfinite = false;
	GConfig->GetBool(TEXT("CustomVariables"), TEXT("InfiniteHealth"), isHealthInfinite, GGameIni);

	FAggregatorEvaluateParameters EvaluationParameters;

	float BulletCount;
	GetCapturedAttributeMagnitude(BulletCountDef, Spec, EvaluationParameters, BulletCount);

	if (MyVariables::AllowInfiniteAmmoForGAS)
	{
		return 0;
	}

	// Ensure that we don't set health to a too low value
	constexpr float DecreaseMagnitude = 1;
	return (BulletCount < DecreaseMagnitude) ? 0 : -DecreaseMagnitude;
}
