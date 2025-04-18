// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorExtensionCommands.h"

#define LOCTEXT_NAMESPACE "FMyEditorExtensionModule"

void FMyEditorExtensionCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MyEditorExtension", "Bring up MyEditorExtension window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
