// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorExtensionStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMyEditorExtensionStyle::StyleInstance = nullptr;

void FMyEditorExtensionStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMyEditorExtensionStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMyEditorExtensionStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MyEditorExtensionStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMyEditorExtensionStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MyEditorExtensionStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MyEditorExtension")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MyEditorExtension.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FMyEditorExtensionStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMyEditorExtensionStyle::Get()
{
	return *StyleInstance;
}
