// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicBoxWidget.h"

TSharedRef<SWidget> UDynamicBoxWidget::RebuildWidget()
{
    SlateWidget = SNew(SDynamicBoxWidget)
        .MovementBounds(MovementBounds)
        .BoxSize(BoxSize)
        .MoveSpeed(MoveSpeed)
        .RotationSpeed(RotationSpeed);

    return SlateWidget.ToSharedRef();
}

void UDynamicBoxWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    SlateWidget.Reset();
}
