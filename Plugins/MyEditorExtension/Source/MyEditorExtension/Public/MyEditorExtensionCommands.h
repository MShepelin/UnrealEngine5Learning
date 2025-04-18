// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MyEditorExtensionStyle.h"

class FMyEditorExtensionCommands : public TCommands<FMyEditorExtensionCommands>
{
public:

	FMyEditorExtensionCommands()
		: TCommands<FMyEditorExtensionCommands>(TEXT("MyEditorExtension"), NSLOCTEXT("Contexts", "MyEditorExtension", "MyEditorExtension Plugin"), NAME_None, FMyEditorExtensionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};