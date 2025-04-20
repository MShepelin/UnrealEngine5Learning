// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WebImageActor.generated.h"

UCLASS()
class MYPROJECT_API AWebImageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWebImageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GetImageAsTexture();

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	UTexture2D* WebTexture;
};
