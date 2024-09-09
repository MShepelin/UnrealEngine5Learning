// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAttributes.h"

UHealthAttributes::UHealthAttributes()
	: IsHealthInfinite(false)
{
	GConfig->GetBool(TEXT("CustomVariables"), TEXT("InfiniteHealth"), IsHealthInfinite, GGameIni);
}
