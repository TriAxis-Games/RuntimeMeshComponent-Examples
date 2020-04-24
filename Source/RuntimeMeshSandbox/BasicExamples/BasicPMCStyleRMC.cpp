// Copyright 2016-2020 Chris Conway (Koderz). All Rights Reserved.

#include "BasicPMCStyleRMC.h"
#include "RuntimeMeshProviderStatic.h"



ABasicPMCStyleRMC::ABasicPMCStyleRMC()
	: Material(nullptr)
{

}

void ABasicPMCStyleRMC::OnConstruction(const FTransform& Transform)
{
 	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this, TEXT("RuntimeMeshProvider-Static"));
 	if (StaticProvider)
 	{
 		// The static provider should initialize before we use it
 		GetRuntimeMeshComponent()->Initialize(StaticProvider);
 
 		StaticProvider->SetupMaterialSlot(0, TEXT("TriMat"), Material);
 
 
 		// This creates 3 positions for a triangle
 		TArray<FVector> Positions{ FVector(0, -50, 0), FVector(0, 0, 100), FVector(0, 50, 0) };
 
 		// This creates 3 vertex colors
 		TArray<FColor> Colors{ FColor::Blue, FColor::Red, FColor::Green };
 
 		// This indexes our simple triangle
 		TArray<int32> Triangles = { 0, 1, 2 };
 
 		TArray<FVector> EmptyNormals;
 		TArray<FVector2D> EmptyTexCoords;
 		TArray<FRuntimeMeshTangent> EmptyTangents;
 		StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, EmptyNormals, EmptyTexCoords, Colors, EmptyTangents, ERuntimeMeshUpdateFrequency::Infrequent, true);
 	}
}
