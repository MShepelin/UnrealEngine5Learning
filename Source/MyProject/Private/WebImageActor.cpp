// Fill out your copyright notice in the Description page of Project Settings.


#include "WebImageActor.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

// Sets default values
AWebImageActor::AWebImageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWebImageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWebImageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWebImageActor::GetImageAsTexture()
{
	TArray<uint8> ImageData;

	FString FilePath = FPaths::ProjectDir();
	FilePath.Append(TEXT("Images/blueprintVMbreakpoint.png"));

	if (FFileHelper::LoadFileToArray(ImageData, *FilePath))
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
		{
			TArray<uint8> UncompressedBGRA;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				WebTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
				void* TextureData = WebTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
				WebTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
				WebTexture->UpdateResource();
			}
		}
	}
}
