// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiversionSaveGame.h"

const FGuid MULTIVERSION_SAVE_GUID(0x3F2504E0, 0x4F8941D3, 0x9A0C0305, 0xE82C3301);

FCustomVersionRegistration GRegisterSaveVersion(
	MULTIVERSION_SAVE_GUID,
	static_cast<int>(Versions::Latest),
	TEXT("MultiversionSaveSystem")
);

UMultiversionSaveGame::UMultiversionSaveGame()
	: Bullets(0)
{
}

void UMultiversionSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(MULTIVERSION_SAVE_GUID);
	const int CurrentVersion = Ar.CustomVer(MULTIVERSION_SAVE_GUID);

	if (CurrentVersion >= static_cast<int>(Versions::BaseVersion))
	{
		Ar << PlayerTransform;
	}

	if (CurrentVersion >= static_cast<int>(Versions::BulletsIncluded))
	{
		Ar << Bullets;
	}
}
