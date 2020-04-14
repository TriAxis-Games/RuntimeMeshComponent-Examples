// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionDisplayProvider.h"

float FFunctionDisplayProviderProxy::function(float x, float y)
{
	return FMath::Sin(sqrt(x*x+y*y) +time);
}

FFunctionDisplayProviderProxy::FFunctionDisplayProviderProxy(TWeakObjectPtr<URuntimeMeshProvider> InParent)
	:FRuntimeMeshProviderProxy(InParent)
{
	minx = -PI*2; maxx = PI*2; miny = minx; maxy = maxx; minvalue = -1; maxvalue = 1;
	pointsx = 100; pointsy = pointsx;
	sizex = 1000; sizey = 1000; sizez = 200;
}

FFunctionDisplayProviderProxy::~FFunctionDisplayProviderProxy()
{
}

void FFunctionDisplayProviderProxy::UpdateProxyParameters(URuntimeMeshProvider * ParentProvider, bool bIsInitialSetup)
{
	UFunctionDisplayProvider* CastParent = (UFunctionDisplayProvider*)ParentProvider;
	//UE_LOG(LogTemp, Log, TEXT("[FFunctionDisplayProviderProxy::UpdateProxyParameters] Updating params"));
	if (CastParent)
	{
		Material = CastParent->Material;
		time = CastParent->time;
		if (!bIsInitialSetup)
		{
			MarkSectionDirty(0, 0);
			MarkCollisionDirty(); //just in case
		}
	}
}

void FFunctionDisplayProviderProxy::Initialize()
{
	SetupMaterialSlot(0, FName("Material"), Material.Get());
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
	Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Average;
	CreateSection(0, 0, Properties);
}

bool FFunctionDisplayProviderProxy::GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData & MeshData)
{
	check(LODIndex == 0 && SectionId == 0);
	//UE_LOG(LogTemp, Log, TEXT("[FFunctionDisplayProviderProxy::GetSectionMeshForLOD] Grabbing mesh"));
	float dx = (maxx - minx) / (float)pointsx; //change of x between two points
	float dy = (maxy - miny) / (float)pointsy; //change of y between two points
	for (int32 yindex = 0; yindex < pointsy; yindex++)
	{
		float yalpha = (float)yindex / (float)(pointsy - 1);
		float ypos = FMath::Lerp(-sizey / 2, sizey / 2, yalpha);
		float yvalue = FMath::Lerp(miny, maxy, yalpha);
		for (int32 xindex = 0; xindex < pointsx; xindex++)
		{
			float xalpha = (float)xindex / (float)(pointsx - 1);
			float xpos = FMath::Lerp(-sizex / 2, sizex / 2, xalpha);
			float xvalue = FMath::Lerp(minx, maxx, xalpha);

			FVector Position(xpos, ypos, 
				FMath::GetMappedRangeValueClamped(
					FVector2D(minvalue, maxvalue), 
					FVector2D(-sizez / 2, sizez / 2), 
					function(xvalue, yvalue)
				)
			);

			float dfdx = (function(xvalue + dx, yvalue) - function(xvalue - dx, yvalue)) / (2 * dx); //derivative of f over x
			float dfdy = (function(xvalue, yvalue + dy) - function(xvalue, yvalue - dy)) / (2 * dy); //derivative of f over y
			
			FVector Normal(-dfdx, -dfdy, 1);
			Normal.Normalize();
			FVector Tangent(1, 0, dfdx);
			Tangent.Normalize();

			FVector2D UV(xalpha, yalpha);

			MeshData.Positions.Add(Position);
			MeshData.Tangents.Add(Normal, Tangent);
			MeshData.TexCoords.Add(UV);
			MeshData.Colors.Add(FColor::White);

			if (xindex != pointsx - 1 && yindex != pointsy - 1)
			{
				int32 AIndex = xindex + yindex * pointsx;
				int32 BIndex = AIndex + 1;
				int32 CIndex = AIndex + pointsx;
				int32 DIndex = CIndex + 1;
				MeshData.Triangles.AddTriangle(AIndex, CIndex, BIndex);
				MeshData.Triangles.AddTriangle(BIndex, CIndex, DIndex);
			}
		}
	}
	return true;
}

FBoxSphereBounds FFunctionDisplayProviderProxy::GetBounds()
{
	FVector ext(sizex, sizey, sizez);
	FBox box = FBox(-ext / 2, ext / 2);
	return FBoxSphereBounds(box);
}

UFunctionDisplayProvider::UFunctionDisplayProvider()
{

}

void UFunctionDisplayProvider::SetTime(float Intime)
{
	//UE_LOG(LogTemp, Log, TEXT("[UFunctionDisplayProvider::SetTime] Called"));
	time = Intime;
	MarkProxyParametersDirty();
}

FRuntimeMeshProviderProxyRef UFunctionDisplayProvider::GetProxy()
{
	return MakeShared<FFunctionDisplayProviderProxy, ESPMode::ThreadSafe>(TWeakObjectPtr<URuntimeMeshProvider>(this));
}
