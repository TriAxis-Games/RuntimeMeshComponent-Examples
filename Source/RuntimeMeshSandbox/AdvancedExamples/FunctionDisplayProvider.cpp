// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionDisplayProvider.h"


UFunctionDisplayProvider::UFunctionDisplayProvider()
{
	MinX = -PI * 2; MaxX = PI * 2; MinY = MinX; MaxY = MaxX; MinValue = -1; MaxValue = 1;
	PointsSX = 128; PointsSY = PointsSX;
	SizeX = 1000; SizeY = 1000; SizeZ = 200;

	CalculateBounds();
}

UMaterialInterface* UFunctionDisplayProvider::GetDisplayMaterial() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return DisplayMaterial;
}

void UFunctionDisplayProvider::SetDisplayMaterial(UMaterialInterface* InMaterial)
{
	FScopeLock Lock(&PropertySyncRoot);
	DisplayMaterial = InMaterial;
}

float UFunctionDisplayProvider::GetTime() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return Time;
}

void UFunctionDisplayProvider::SetTime(float InTime)
{
	{
		FScopeLock Lock(&PropertySyncRoot);
		Time = InTime;
	}
	MarkLODDirty(0);
}

void UFunctionDisplayProvider::Initialize()
{
	SetupMaterialSlot(0, FName("Material"), DisplayMaterial);
	TArray<FRuntimeMeshLODProperties> LODs;
	FRuntimeMeshLODProperties LODProperties;
	LODProperties.ScreenSize = 0.0f;
	LODs.Add(LODProperties);
	ConfigureLODs(LODs);

	FRuntimeMeshSectionProperties Properties;
	Properties.bCastsShadow = true;
	Properties.bIsVisible = true;
	Properties.MaterialSlot = 0;
	Properties.bWants32BitIndices = true;
	Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Frequent;
	CreateSection(0, 0, Properties);
}

FBoxSphereBounds UFunctionDisplayProvider::GetBounds()
{
	return LocalBounds;
}

bool UFunctionDisplayProvider::GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData)
{

	check(LODIndex == 0 && SectionId == 0);
	float dx = (MaxX - MinX) / (float)PointsSX; //change of x between two points
	float dy = (MaxY - MinY) / (float)PointsSY; //change of y between two points
	for (int32 yindex = 0; yindex < PointsSY; yindex++)
	{
		float yalpha = (float)yindex / (float)(PointsSY - 1);
		float ypos = FMath::Lerp(-SizeY / 2, SizeY / 2, yalpha);
		float yvalue = FMath::Lerp(MinY, MaxY, yalpha);
		for (int32 xindex = 0; xindex < PointsSX; xindex++)
		{
			float xalpha = (float)xindex / (float)(PointsSX - 1);
			float xpos = FMath::Lerp(-SizeX / 2, SizeX / 2, xalpha);
			float xvalue = FMath::Lerp(MinX, MaxX, xalpha);

			FVector Position(xpos, ypos,
				FMath::GetMappedRangeValueClamped(
					FVector2D(MinValue, MaxValue),
					FVector2D(-SizeZ / 2, SizeZ / 2),
					CalculateHeightForPoint(xvalue, yvalue)
					)
				);

			float dfdx = (CalculateHeightForPoint(xvalue + dx, yvalue) - CalculateHeightForPoint(xvalue - dx, yvalue)) / (2 * dx); //derivative of f over x
			float dfdy = (CalculateHeightForPoint(xvalue, yvalue + dy) - CalculateHeightForPoint(xvalue, yvalue - dy)) / (2 * dy); //derivative of f over y

			FVector Normal(-dfdx, -dfdy, 1);
			Normal.Normalize();
			FVector Tangent(1, 0, dfdx);
			Tangent.Normalize();

			FVector2D UV(xalpha, yalpha);

			MeshData.Positions.Add(Position);
			MeshData.Tangents.Add(Normal, Tangent);
			MeshData.TexCoords.Add(UV);
			MeshData.Colors.Add(FColor::White);

			if (xindex != PointsSX - 1 && yindex != PointsSY - 1)
			{
				int32 AIndex = xindex + yindex * PointsSX;
				int32 BIndex = AIndex + 1;
				int32 CIndex = AIndex + PointsSX;
				int32 DIndex = CIndex + 1;
				MeshData.Triangles.AddTriangle(AIndex, CIndex, BIndex);
				MeshData.Triangles.AddTriangle(BIndex, CIndex, DIndex);
			}
		}
	}
	return true;
}

bool UFunctionDisplayProvider::IsThreadSafe()
{
	return true;
}

void UFunctionDisplayProvider::CalculateBounds()
{
	FVector Ext(SizeX, SizeY, SizeZ);
	FBox Box = FBox(-Ext / 2, Ext / 2);
	LocalBounds = FBoxSphereBounds(Box);
}

float UFunctionDisplayProvider::CalculateHeightForPoint(float x, float y)
{
	return FMath::Sin(sqrt(x * x + y * y) + Time);
}
