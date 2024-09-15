// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASHelpers.h"
#include "GravityGunAttributes.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGravityGunAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UGravityGunAttributes() = default;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttributeData CurrentGravityGunDistance;
	ATTRIBUTE_ACCESSORS(UGravityGunAttributes, CurrentGravityGunDistance)
};
