// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Invariant: HealthPoints is INFINITE_HEALTH or 0 or greater.
	UPROPERTY(EditDefaultsOnly, Category = "Default", Meta = (ClampMin = "0", ClampMax = "100"))
	int32 HealthPoints;

public:
	const int32 INFINITE_HEALTH = -1;

	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintPure)
	int32 GetHealthPointsCheap() const;

	// This function is used to test Pure function modifier in Unreal Engine. 
	// Functionally it's the same as GetHealthPointsCheap.
	UFUNCTION(BlueprintPure)
	int32 GetHealthPointsExpensive();

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(int32 healthDelta);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CheckIfOwnerShouldBeDestroyed();
};
