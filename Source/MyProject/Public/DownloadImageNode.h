// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "DownloadImageNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImageDownloaded, UTexture2D*, DownloadedTexture);

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDownloadImageNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UDownloadImageNode* DownloadImage(const FString& URL);

	UPROPERTY(BlueprintAssignable)
	FOnImageDownloaded Completed;

private:
	void HandleImageDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY()
	FString ImageURL;
};
