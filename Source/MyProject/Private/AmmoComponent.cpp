// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoComponent.h"
#include "HAL/IConsoleManager.h"


namespace MyVariables
{
	bool AllowInfiniteAmmo = false;
	FAutoConsoleVariableRef AllowInfiniteAmmoConsoleRef(
		TEXT("cvar.infi_ammo"),
		AllowInfiniteAmmo,
		TEXT("Allows infinite ammo for everyone using ammo component"),
		ECVF_Default
	);
};


// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UAmmoComponent::TryToUseBullet()
{
	if (BulletCount > 0)
	{
		--BulletCount;
		return true;
	}
	else if (MyVariables::AllowInfiniteAmmo)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UAmmoComponent::AddBullets(int32 Bullets)
{
	BulletCount += Bullets;
}
