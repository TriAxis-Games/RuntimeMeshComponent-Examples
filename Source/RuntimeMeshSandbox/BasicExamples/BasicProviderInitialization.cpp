// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#include "BasicProviderInitialization.h"
#include "Providers/RuntimeMeshProviderBox.h"



ABasicProviderInitialization::ABasicProviderInitialization()
	: Material(nullptr)
{

}

void ABasicProviderInitialization::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	URuntimeMeshProviderBox* BoxProvider = NewObject<URuntimeMeshProviderBox>(this, TEXT("RuntimeMeshProvider-Box"));
	if (BoxProvider)
	{
		BoxProvider->BoxRadius = FVector(100, 100, 100);
		BoxProvider->Material = Material;

		GetRuntimeMeshComponent()->Initialize(BoxProvider);
	}
}
