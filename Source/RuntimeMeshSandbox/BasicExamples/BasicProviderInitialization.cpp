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
		BoxProvider->SetBoxRadius(FVector(100, 100, 100));
		BoxProvider->SetBoxMaterial(Material);

		GetRuntimeMeshComponent()->Initialize(BoxProvider);
	}
}
