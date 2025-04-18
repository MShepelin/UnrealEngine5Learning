// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorExtension.h"
#include "MyEditorExtensionStyle.h"
#include "MyEditorExtensionCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Engine/StaticMeshActor.h"
#include "ToolMenus.h"

static const FName MyEditorExtensionTabName("MyEditorExtension");

#define LOCTEXT_NAMESPACE "FMyEditorExtensionModule"

void FMyEditorExtensionModule::StartupModule()
{
	FMyEditorExtensionStyle::Initialize();
	FMyEditorExtensionStyle::ReloadTextures();

	FMyEditorExtensionCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMyEditorExtensionCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMyEditorExtensionModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMyEditorExtensionModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MyEditorExtensionTabName, FOnSpawnTab::CreateRaw(this, &FMyEditorExtensionModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMyEditorExtensionTabTitle", "MyEditorExtension"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMyEditorExtensionModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MyEditorExtensionTabName);

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMyEditorExtensionCommands::Unregister();

	FMyEditorExtensionStyle::Shutdown();
}

TSharedRef<SDockTab> FMyEditorExtensionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const FText WidgetText = FText::FromString(TEXT("This tool allows you to spawn a sphare with a give scale"));
	const FText RadiusText = FText::FromString(TEXT("Sphere scale"));
	const FText DistanceText = FText::FromString(TEXT("Distance from camera"));
	const FText ButtonText = FText::FromString(TEXT("Spawn a sphere in front of the camera"));

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(FMargin(20.0f, 20.0f, 20.0f, 20.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(WidgetText)
				]
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(0.5f)
					[
						SNew(SBox)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(RadiusText)
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(0.5f)
					[
						SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.MinValue(1.f)
						.Value_Lambda([this] { return GetSphereScale(); })
						.OnValueChanged_Lambda([this](float NewValue) { FMyEditorExtensionModule::OnSphereScaleChange(NewValue); })
					]
				]
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(0.5f)
					[
						SNew(SBox)
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text(DistanceText)
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(0.5f)
					[
						SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.MinValue(1.f)
						.Value_Lambda([this] { return GetDistanceFromCamera(); })
						.OnValueChanged_Lambda([this](float NewValue) { FMyEditorExtensionModule::OnDistanceFromCameraChange(NewValue); })
					]
				]
				+ SVerticalBox::Slot()
				[
					SNew(SBox)
					.HeightOverride(40.0f)
					[
						SNew(SButton)
						.OnClicked_Lambda([this]() { OnSpawnSphereButtonClicked(); return FReply::Handled(); })
						[
							SNew(STextBlock)
							.Text(ButtonText)
						]
					]
				]
			]
		];
}

void FMyEditorExtensionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MyEditorExtensionTabName);
}

void FMyEditorExtensionModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMyEditorExtensionCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}
}

TOptional<float> FMyEditorExtensionModule::GetSphereScale() const
{
	return SphereScale;
};

void FMyEditorExtensionModule::OnSphereScaleChange(float NewValue)
{
	SphereScale = NewValue;
}

TOptional<float> FMyEditorExtensionModule::GetDistanceFromCamera() const
{
	return DistanceFromCamera;
}

void FMyEditorExtensionModule::OnDistanceFromCameraChange(float NewValue)
{
	DistanceFromCamera = NewValue;
}

void FMyEditorExtensionModule::OnSpawnSphereButtonClicked()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World == nullptr)
	{
		return;
	}

	FViewport* Viewport = GEditor->GetActiveViewport();
	if (Viewport)
	{
		FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
		if (ViewportClient)
		{
			const FVector CameraLocation = ViewportClient->GetViewLocation();
			const FRotator CameraRotation = ViewportClient->GetViewRotation();

			const FVector SphereSpawnPosition = CameraLocation + CameraRotation.Vector() * DistanceFromCamera;

			AStaticMeshActor* SphereActor = World->SpawnActor<AStaticMeshActor>(SphereSpawnPosition, CameraRotation);
			UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			SphereActor->GetStaticMeshComponent()->SetStaticMesh(SphereMesh);
			SphereActor->SetActorScale3D(FVector(SphereScale));
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyEditorExtensionModule, MyEditorExtension)