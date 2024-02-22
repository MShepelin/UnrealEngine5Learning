// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MyCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(exec)
	void ReloadGameConfig()
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Reloading game config"));
		FString fileName = GGameIni;
		GConfig->LoadGlobalIniFile(fileName, *GGameIni, NULL, true);
	}
};
