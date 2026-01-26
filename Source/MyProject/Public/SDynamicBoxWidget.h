#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SDynamicBoxWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SDynamicBoxWidget)
        : _MovementBounds(FVector2D(300.f, 300.f))
        , _BoxSize(FVector2D(50.f, 50.f))
        , _MoveSpeed(60.f)
        , _RotationSpeed(90.f)
        {}
        SLATE_ARGUMENT(FVector2D, MovementBounds)
        SLATE_ARGUMENT(FVector2D, BoxSize)
        SLATE_ARGUMENT(float, MoveSpeed)
        SLATE_ARGUMENT(float, RotationSpeed)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    virtual int32 OnPaint(
        const FPaintArgs& Args,
        const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId,
        const FWidgetStyle& InWidgetStyle,
        bool bParentEnabled
    ) const override;

    virtual void Tick(
        const FGeometry& AllottedGeometry,
        const double InCurrentTime,
        const float InDeltaTime
    ) override;

private:
    FVector2D Bounds;
    FVector2D Size;
    float MoveSpeed;
    float RotationSpeed;

    mutable FVector2D Position;
    mutable FVector2D Velocity;
    mutable float Rotation;
};
