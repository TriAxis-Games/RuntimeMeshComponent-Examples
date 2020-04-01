// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "BasicProviderInitialization.generated.h"

/**
 * 
 */
UCLASS()
class RUNTIMEMESHSANDBOX_API ABasicProviderInitialization : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;
	
public:
	ABasicProviderInitialization();


	void OnConstruction(const FTransform& Transform) override;

};
