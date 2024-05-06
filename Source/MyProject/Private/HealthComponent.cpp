// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UHealthComponent::CheckIfOwnerShouldBeDestroyed()
{
	if (HealthPoints == 0)
	{
		AActor* owner = GetOwner();
		if (ensure(owner))
		{
			owner->Destroy();
		}
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	bool isInfiniteHealthAllowed = false;
	GConfig->GetBool(TEXT("CustomVariables"), TEXT("InfiniteHealth"), isInfiniteHealthAllowed, GGameIni);

	if (isInfiniteHealthAllowed)
	{
		HealthPoints = INFINITE_HEALTH;
	}

	CheckIfOwnerShouldBeDestroyed();
}


int32 UHealthComponent::GetHealthPointsCheap() const
{
	return HealthPoints;
}


int32 UHealthComponent::GetHealthPointsExpensive()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Very expensive operation in pure function"));
	}
	return GetHealthPointsCheap();
}


void UHealthComponent::DecreaseHealth(int32 healthDelta) 
{
	ensure(healthDelta >= 0);

	if (HealthPoints == INFINITE_HEALTH)
	{
		return;
	}

	// Invariant: HealthPoints is INFINITE_HEALTH or 0 or greater.
	HealthPoints -= healthDelta;
	if (HealthPoints <= 0)
	{
		HealthPoints = 0;
	}

	CheckIfOwnerShouldBeDestroyed();
}
