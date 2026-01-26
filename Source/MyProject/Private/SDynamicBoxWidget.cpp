#include "SDynamicBoxWidget.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"

void SDynamicBoxWidget::Construct(const FArguments& InArgs)
{
    Bounds = InArgs._MovementBounds;
    Size = InArgs._BoxSize;
    MoveSpeed = InArgs._MoveSpeed;
    RotationSpeed = InArgs._RotationSpeed;

    Position = FVector2D(FMath::FRandRange(0.f, Bounds.X),
        FMath::FRandRange(0.f, Bounds.Y));

    Velocity = FVector2D(FMath::FRandRange(-1.f, 1.f),
        FMath::FRandRange(-1.f, 1.f)).GetSafeNormal() * MoveSpeed;

    Rotation = FMath::FRandRange(0.f, 360.f);
}

void SDynamicBoxWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    Position += Velocity * InDeltaTime;
    Rotation += RotationSpeed * InDeltaTime;

    // Bounce inside bounds
    if (Position.X < 0 || Position.X > Bounds.X)
    {
        Velocity.X *= -1.f;
    }

    if (Position.Y < 0 || Position.Y > Bounds.Y)
    {
        Velocity.Y *= -1.f;
    }

    Position.X = FMath::Clamp(Position.X, 0.f, Bounds.X);
    Position.Y = FMath::Clamp(Position.Y, 0.f, Bounds.Y);
}

int32 SDynamicBoxWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");

    FSlateRenderTransform Transform = FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(Rotation)), Position);

    FSlateDrawElement::MakeBox(OutDrawElements, LayerId,
        AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform{}, Transform),
        WhiteBrush, ESlateDrawEffect::None, FLinearColor::White);

    return LayerId + 1;
}
