// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAttributes.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

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

void UHealthAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributes, HealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributes, MaxHealthPoints, COND_None, REPNOTIFY_Always);
}

void UHealthAttributes::OnRep_HealthPoints(const FGameplayAttributeData& OldHealthPoints)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributes, HealthPoints, OldHealthPoints);
}

void UHealthAttributes::OnRep_MaxHealthPoints(const FGameplayAttributeData& OldMaxHealthPoints)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributes, MaxHealthPoints, OldMaxHealthPoints);
}
