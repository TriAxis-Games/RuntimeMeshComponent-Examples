// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RuntimeMeshSandboxHUD.generated.h"

UCLASS()
class ARuntimeMeshSandboxHUD : public AHUD
{
	GENERATED_BODY()

public:
	ARuntimeMeshSandboxHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

