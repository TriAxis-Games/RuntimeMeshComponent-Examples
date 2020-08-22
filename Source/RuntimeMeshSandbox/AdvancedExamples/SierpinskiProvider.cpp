// Fill out your copyright notice in the Description page of Project Settings.


#include "SierpinskiProvider.h"
#include "RuntimeMeshModifierNormals.h"

USierpinskiProvider::USierpinskiProvider()
{
	TetrahedronRadius = 100.f;
	MaxIterations = 6;
	ScreenSizeReason = 0.33f;
}

float USierpinskiProvider::GetTetrahedronRadius() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return TetrahedronRadius;
}

void USierpinskiProvider::SetTetrahedronRadius(const float & InRadius)
{
	FScopeLock Lock(&PropertySyncRoot);
	TetrahedronRadius = InRadius;

	//Mesh needs to be rebuilt
	MarkAllLODsDirty();
	MarkCollisionDirty();
}

int32 USierpinskiProvider::GetMaxIterations() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return MaxIterations;
}

void USierpinskiProvider::SetMaxIterations(const int32 & InMaxIterations)
{
	check(InMaxIterations > 0 && InMaxIterations < 12); //At 12 iterations, that would be 50 331 648 vertices... We don't want that (I think?)
	FScopeLock Lock(&PropertySyncRoot);

	//Remove superfluous sections
	if (InMaxIterations < MaxIterations)
	{
		for (int32 LODIndex = InMaxIterations; LODIndex < MaxIterations; LODIndex++)
		{
			RemoveSection(LODIndex, 0);
		}
	}

	//Reconfigure LODs
	TArray<FRuntimeMeshLODProperties> LODConfigurations;
	for (int32 LODIndex = 0; LODIndex < InMaxIterations; LODIndex++)
	{
		FRuntimeMeshLODProperties LOD;
		if (LODIndex != InMaxIterations - 1)
		{
			LOD.ScreenSize = FMath::Pow(ScreenSizeReason, LODIndex + 1);
		}
		else
		{
			LOD.ScreenSize = 0.0f;
		}
	}
	ConfigureLODs(LODConfigurations);

	//Add missing sections and update bWants32bitIndices
	for (int32 LODIndex = 0; LODIndex < InMaxIterations; LODIndex++)
	{
		FRuntimeMeshSectionProperties Properties;
		Properties.bCastsShadow = true;
		Properties.bIsVisible = true;
		Properties.MaterialSlot = 0;
		Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;
		Properties.bWants32BitIndices = (3<<(2*(InMaxIterations - LODIndex +1))) >= 1 << 16; //4^n = 2^2n = 1<<2n
		CreateSection(LODIndex, 0, Properties);
	}
	MaxIterations = InMaxIterations;
	MarkAllLODsDirty();
}

float USierpinskiProvider::GetScreenSizeReason() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return ScreenSizeReason;
}

void USierpinskiProvider::SetScreenSizeReason(const float & InScreenSizeReason)
{
	FScopeLock Lock(&PropertySyncRoot);
	ScreenSizeReason = InScreenSizeReason;

	//Reconfigure the LODs to use the new reason
	TArray<FRuntimeMeshLODProperties> LODConfigurations;
	for (int32 LODIndex = 0; LODIndex < MaxIterations; LODIndex++)
	{
		FRuntimeMeshLODProperties LOD;
		if (LODIndex != MaxIterations - 1)
		{
			LOD.ScreenSize = FMath::Pow(ScreenSizeReason, LODIndex + 1);
		}
		else
		{
			LOD.ScreenSize = 0.0f;
		}
	}
	ConfigureLODs(LODConfigurations);
}

UMaterialInterface * USierpinskiProvider::GetSierpinskiMaterial() const
{
	FScopeLock Lock(&PropertySyncRoot);
	return Material;
}

void USierpinskiProvider::SetSierpinskiMaterial(UMaterialInterface * InMaterial)
{
	FScopeLock Lock(&PropertySyncRoot);
	Material = InMaterial;
	SetupMaterialSlot(0, FName("Base"), Material);
}
//A,B,C and D are the edges of the tetrahedron
//Once SubdivisionIndex reaches 0, it makes a tetrahedron, otherwise it subdivides and removes 1 to SubdivisionIndex
void USierpinskiProvider::AddTetrahedronOrSubdivide(FVector A, FVector B, FVector C, FVector D, int32 SubdivisionIndex, FRuntimeMeshRenderableMeshData & MeshData)
{
	if (SubdivisionIndex <= 0)
	{
		auto AddTriangle = [&](const FVector& At, const FVector& Bt, const FVector& Ct)
		{
			int32 NumVerts = MeshData.Positions.Num();
			FVector normal = ((Bt - At) ^ (Ct - At)).GetSafeNormal();
			MeshData.Positions.Append({ At, Bt, Ct});
			for (int32 i = 0; i < 3; i++)
			{
				MeshData.Tangents.Add(normal, FVector::ZeroVector);
				MeshData.Colors.Add(FColor::White);
				MeshData.TexCoords.Add(FVector2D::ZeroVector);
			}
			MeshData.Triangles.AddTriangle(NumVerts, NumVerts + 1, NumVerts + 2);
		};

		//Add tetra

		AddTriangle(A, B, C);
		AddTriangle(A, D, B);
		AddTriangle(A, C, D);
		AddTriangle(C, B, D);
	}
	else
	{
		FVector AB = (A + B) / 2.f;
		FVector AC = (A + C) / 2.f;
		FVector AD = (A + D) / 2.f;
		FVector BC = (B + C) / 2.f;
		FVector BD = (B + D) / 2.f;
		FVector CD = (C + D) / 2.f;
		AddTetrahedronOrSubdivide(A, AB, AC, AD, SubdivisionIndex - 1, MeshData);
		AddTetrahedronOrSubdivide(AB, B, BC, BD, SubdivisionIndex - 1, MeshData);
		AddTetrahedronOrSubdivide(AC, BC, C, CD, SubdivisionIndex - 1, MeshData);
		AddTetrahedronOrSubdivide(AD, BD, CD, D, SubdivisionIndex - 1, MeshData);
	}
}

void USierpinskiProvider::Initialize()
{
	TArray<FRuntimeMeshLODProperties> LODConfigurations;
	for (int32 LODIndex = 0; LODIndex < MaxIterations; LODIndex++)
	{
		FRuntimeMeshLODProperties LOD;
		if (LODIndex != MaxIterations -1)
		{
			LOD.ScreenSize = FMath::Pow(ScreenSizeReason, LODIndex + 1);
		}
		else
		{
			LOD.ScreenSize = 0.0f;
		}
		LODConfigurations.Add(LOD);
	}
	ConfigureLODs(LODConfigurations);

	SetupMaterialSlot(0, FName("Base"), GetSierpinskiMaterial());

	for (int32 LODIndex = 0; LODIndex < MaxIterations; LODIndex++)
	{
		FRuntimeMeshSectionProperties Properties;
		Properties.bCastsShadow = true;
		Properties.bIsVisible = true;
		Properties.MaterialSlot = 0;
		Properties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;
		Properties.bWants32BitIndices = (3 << (2 * (MaxIterations - LODIndex))) >= 1 << 16; //Automatic 32 bit indices
		CreateSection(LODIndex, 0, Properties);
	}
}

FBoxSphereBounds USierpinskiProvider::GetBounds()
{
	return FBoxSphereBounds(FSphere(FVector::ZeroVector, TetrahedronRadius));
}

//A tetrahedron has 4 triangles, so 12 vertices
//Each time we subdivide, we add 4 more of each
//So in total, we have 3*4^subdivisions = 3*4^(MaxIterations-LODIndex) = 3<<(2*(MaxIterations-LODIndex))
//<< is a bitshift left, that's faster than just power and simpler for powers of two

bool USierpinskiProvider::GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData & MeshData)
{
	float TempTetrahedronRadius; int32 TempMaxIterations;
	{
		FScopeLock Lock(&PropertySyncRoot);
		TempTetrahedronRadius = TetrahedronRadius;
		TempMaxIterations = MaxIterations;
	}
	check(LODIndex < TempMaxIterations && SectionId == 0);
	FVector A(FMath::Sqrt(8.f / 9.f), 0, -1.f / 3.f);
	FVector B(-FMath::Sqrt(2.f / 9.f), FMath::Sqrt(2.f / 3.f), -1.f / 3.f);
	FVector C(-FMath::Sqrt(2.f / 9.f), -FMath::Sqrt(2.f / 3.f), -1.f / 3.f);
	FVector D(0.f, 0.f, 1.f);
	MeshData.ReserveVertices(3 << (2 * (TempMaxIterations - LODIndex)));
	MeshData.Triangles.Reserve(3 << (2 * (TempMaxIterations - LODIndex)));
	AddTetrahedronOrSubdivide(A*TetrahedronRadius, B*TetrahedronRadius, C*TetrahedronRadius, D*TetrahedronRadius, TempMaxIterations - LODIndex, MeshData);
	URuntimeMeshModifierNormals::CalculateNormalsTangents(MeshData);
	return true;
}

FRuntimeMeshCollisionSettings USierpinskiProvider::GetCollisionSettings()
{
	return FRuntimeMeshCollisionSettings();
}

bool USierpinskiProvider::HasCollisionMesh()
{
	return false;
}

bool USierpinskiProvider::GetCollisionMesh(FRuntimeMeshCollisionData & CollisionData)
{
	return false;
}

bool USierpinskiProvider::IsThreadSafe()
{
	return true;
}
