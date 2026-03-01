#include "ConqueringData.h"

FConqueringData::FConqueringData()
    : ConquerringPoints(0)
    , ConquerringMaxPoints(0)
{
}

bool FConqueringData::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& OutSuccess)
{
    constexpr uint16 UsedBits = 7;
    constexpr uint16 UsedBitsMask = 0x7F;
    uint16 Packed = (ConquerringPoints & UsedBitsMask) << UsedBits | (ConquerringMaxPoints & UsedBitsMask);

    if (Ar.IsSaving())
    {
        Ar.SerializeBits(&Packed, UsedBits + UsedBits);
    }
    else
    {
        Ar.SerializeBits(&Packed, UsedBits + UsedBits);
        // Extra clamp to account for incorrect data being received
        ConquerringPoints = FMath::Clamp<uint8>((Packed >> UsedBits) & UsedBitsMask, 0, 100);
        ConquerringMaxPoints = FMath::Clamp<uint8>(Packed & UsedBitsMask, 0, 100);
    }

    OutSuccess = true;
    return true;
}
