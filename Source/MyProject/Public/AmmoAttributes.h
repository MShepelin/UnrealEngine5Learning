// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASHelpers.h"
#include "AmmoAttributes.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UAmmoAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttributeData BulletCount;
	ATTRIBUTE_ACCESSORS(UAmmoAttributes, BulletCount)
};
