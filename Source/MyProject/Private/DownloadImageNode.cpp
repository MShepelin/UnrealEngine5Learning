#include "DownloadImageNode.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"

UDownloadImageNode* UDownloadImageNode::DownloadImage(const FString& URL)
{
    UDownloadImageNode* Action = NewObject<UDownloadImageNode>();
    Action->ImageURL = URL;
    return Action;
}

void UDownloadImageNode::Activate()
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ImageURL);
    Request->SetVerb(TEXT("GET"));
    Request->OnProcessRequestComplete().BindUObject(this, &UDownloadImageNode::HandleImageDownloaded);
    Request->ProcessRequest();
}

void UDownloadImageNode::HandleImageDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    UTexture2D* Texture = nullptr;

    if (bWasSuccessful && Response.IsValid())
    {
        const TArray<uint8>& ImageData = Response->GetContent();

        IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
        TSharedPtr<IImageWrapper> Wrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

        if (Wrapper.IsValid() && Wrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
        {
            TArray<uint8> UncompressedBGRA;
            if (Wrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
            {
                Texture = UTexture2D::CreateTransient(Wrapper->GetWidth(), Wrapper->GetHeight(), PF_B8G8R8A8);
                void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
                Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
                Texture->UpdateResource();
            }
        }
    }
    
    Completed.Broadcast(Texture);
}
