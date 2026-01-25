// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConqueredArea.generated.h"

UCLASS()
class MYPROJECT_API AConqueredArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConqueredArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnConquerringPointsChanged(int NewConquerringPoints, int MaxConquerringPoints);

	virtual void OnConquerringPointsChanged_Implementation(int NewConquerringPoints, int MaxConquerringPoints);

	UFUNCTION(BlueprintCallable)
	void AddConqueringPoint();


	UPROPERTY(ReplicatedUsing = OnRep_ConquerringPoints)
	int ConquerringPoints = 0;

	// This value should be defined in defaults of the class
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int ConquerringMaxPoints = 10;

private:
	UFUNCTION()
	virtual void OnRep_ConquerringPoints(int OldConquerringPoints);
};
