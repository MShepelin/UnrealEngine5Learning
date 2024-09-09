// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASHelpers.h"
#include "HealthAttributes.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHealthAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHealthAttributes();

	UFUNCTION(BlueprintCallable)
	bool HasInfiniteHealth() const
	{
		return IsHealthInfinite;
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttributeData HealthPoints;
	ATTRIBUTE_ACCESSORS(UHealthAttributes, HealthPoints)

private:
	UPROPERTY()
	bool IsHealthInfinite;
};
