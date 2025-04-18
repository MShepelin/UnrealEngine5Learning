// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMyEditorExtensionModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void PluginButtonClicked();
	
private:
	void RegisterMenus();

	TOptional<float> GetSphereScale() const;
	void OnSphereScaleChange(float NewValue);
	TOptional<float> GetDistanceFromCamera() const;
	void OnDistanceFromCameraChange(float NewValue);
	void OnSpawnSphereButtonClicked();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<class FUICommandList> PluginCommands;
	float SphereScale = 10.f;
	float DistanceFromCamera = 100.f;
};
