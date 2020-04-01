// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#include "RuntimeMeshSandboxGameMode.h"
#include "RuntimeMeshSandboxHUD.h"
#include "RuntimeMeshSandboxCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARuntimeMeshSandboxGameMode::ARuntimeMeshSandboxGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARuntimeMeshSandboxHUD::StaticClass();
}
