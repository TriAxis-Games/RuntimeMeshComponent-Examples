// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "BasicPMCStyleRMC.generated.h"

/**
 * 
 */
UCLASS()
class RUNTIMEMESHSANDBOX_API ABasicPMCStyleRMC : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

public:
	ABasicPMCStyleRMC();


	void OnConstruction(const FTransform& Transform) override;
	
};
