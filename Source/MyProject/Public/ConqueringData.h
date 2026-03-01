#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ConqueringData.generated.h"

USTRUCT(BlueprintType)
struct FConqueringData
{
    GENERATED_BODY()

public:
    FConqueringData();

    bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& OutSuccess);

    UPROPERTY(meta = (ClampMin = "0", ClampMax = "100"))
    uint8 ConquerringPoints;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100"))
    uint8 ConquerringMaxPoints;
};

template<>
struct TStructOpsTypeTraits<FConqueringData> : public TStructOpsTypeTraitsBase2<FConqueringData>
{
    enum
    {
        WithNetSerializer = true
    };
};
