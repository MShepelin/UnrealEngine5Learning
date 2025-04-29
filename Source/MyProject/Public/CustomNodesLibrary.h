// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputAction.h"
#include "CustomNodesLibrary.generated.h"

UCLASS()
class MYPROJECT_API UInputWaiter : public UObject
{
    GENERATED_BODY()

public:
    int32 LatentActionUUID = -1;

    void Initialize(UEnhancedInputComponent* InputComp, UInputAction* InputAction, int32 InUUID);

    UFUNCTION()
    void HandleInputPressed(const FInputActionInstance& Instance);
};

UCLASS()
class MYPROJECT_API UCustomNodesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(Latent, LatentInfo="LatentInfo"))
	static void CustomDelay(APlayerController* Controller, UInputAction* InputAction, float Duration, FLatentActionInfo LatentInfo);

    UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "OutValue"))
    static void GetObjectProperty(UObject* Target, FName PropertyName, int32& OutValue);
    DECLARE_FUNCTION(execGetObjectProperty);
};
