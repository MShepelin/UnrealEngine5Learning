// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MultiversionSaveGame.generated.h"

enum class Versions
{
	BaseVersion = 1,
	BulletsIncluded = 2,
	Latest = BulletsIncluded
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMultiversionSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMultiversionSaveGame();

	UPROPERTY(BlueprintReadWrite)
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite)
	int Bullets = 0;

	virtual void Serialize(FArchive& Ar) override;
};
