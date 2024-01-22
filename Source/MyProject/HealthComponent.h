// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", Meta = (ClampMin = "0", ClampMax = "100"))
	int32 HealthPoints;

	UFUNCTION(BlueprintCallable)
	float GetHealthPointsCheap() const {
		return HealthPoints;
	}

	UFUNCTION(BlueprintPure)
	float GetHealthPointsExpensive();

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float healthDelta);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
