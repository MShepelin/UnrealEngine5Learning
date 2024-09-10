// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAttributes.h"
#include "GameplayEffectExtension.h"

void UHealthAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthPointsAttribute())
    {
        const float NewHealth = GetHealthPoints();
        if (NewHealth <= 0.0f)
        {
            AActor* ownerActor = GetOwningActor();
            if (ensure(ownerActor))
            {
                ownerActor->Destroy();
            }
        }
    }
}
