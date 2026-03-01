// Fill out your copyright notice in the Description page of Project Settings.


#include "ConqueredArea.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AConqueredArea::AConqueredArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AConqueredArea::BeginPlay()
{
	Super::BeginPlay();
	
	OnConquerringPointsChanged(ConqueringStatus.ConquerringPoints, ConqueringStatus.ConquerringMaxPoints);
}

// Called every frame
void AConqueredArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConqueredArea::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AConqueredArea, ConqueringStatus);
}

void AConqueredArea::AddConqueringPoint()
{
	if (HasAuthority() && ConqueringStatus.ConquerringPoints < ConqueringStatus.ConquerringMaxPoints)
	{
		++ConqueringStatus.ConquerringPoints;
		OnConquerringPointsChanged(ConqueringStatus.ConquerringPoints, ConqueringStatus.ConquerringMaxPoints);
	}
}

void AConqueredArea::OnRep_ConquerringPoints(FConqueringData OldConqueringStatus)
{
	OnConquerringPointsChanged(ConqueringStatus.ConquerringPoints, ConqueringStatus.ConquerringMaxPoints);
}

void AConqueredArea::OnConquerringPointsChanged_Implementation(int NewConquerringPoints, int MaxConquerringPoints)
{
	UE_LOG(LogTemp, Error, TEXT("AConqueredArea: conquerring points were changed to %d/%d"), NewConquerringPoints, MaxConquerringPoints);
}
