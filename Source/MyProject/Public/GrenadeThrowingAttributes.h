#pragma once

#include "CoreMinimal.h"
#include "GASHelpers.h"
#include "GrenadeThrowingAttributes.generated.h"

UCLASS()
class MYPROJECT_API UGrenadeThrowingAttributes : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayAttributeData Velocity;
	ATTRIBUTE_ACCESSORS(UGrenadeThrowingAttributes, Velocity)
};
