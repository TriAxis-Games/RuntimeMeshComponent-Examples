// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "SierpinskiProvider.generated.h"

/**
 * 
 */
UCLASS()
class RUNTIMEMESHSANDBOX_API USierpinskiProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()
private:
	mutable FCriticalSection PropertySyncRoot;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetTetrahedronRadius, BlueprintSetter = SetTetrahedronRadius)
		float TetrahedronRadius; //Radius of the sphere the base tetrahedron fits into

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetMaxIterations, BlueprintSetter = SetMaxIterations)
		int32 MaxIterations;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetScreenSizeReason, BlueprintSetter = SetScreenSizeReason)
		float ScreenSizeReason; //Min screen size per lod will be this ^ (LODIndex+1)		(That's a geometric series, hence reason)

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetSierpinskiMaterial, BlueprintSetter = SetSierpinskiMaterial)
		UMaterialInterface* Material;
public:
	USierpinskiProvider();

	UFUNCTION(BlueprintCallable)
		float GetTetrahedronRadius() const;
	UFUNCTION(BlueprintCallable)
		void SetTetrahedronRadius(const float& InRadius);

	UFUNCTION(BlueprintCallable)
		int32 GetMaxIterations() const;
	UFUNCTION(BlueprintCallable)
		void SetMaxIterations(const int32& InMaxIterations);

	UFUNCTION(BlueprintCallable)
		float GetScreenSizeReason() const;
	UFUNCTION(BlueprintCallable)
		void SetScreenSizeReason(const float& InScreenSizeReason);

	UFUNCTION(BlueprintCallable)
		UMaterialInterface* GetSierpinskiMaterial() const;
	UFUNCTION(BlueprintCallable)
		void SetSierpinskiMaterial(UMaterialInterface* InMaterial);

private:
	void AddTetrahedronOrSubdivide(FVector A, FVector B, FVector C, FVector D, int32 SubdivisionIndex, FRuntimeMeshRenderableMeshData& MeshData);

protected:
	void Initialize() override;
	FBoxSphereBounds GetBounds() override;
	bool GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	FRuntimeMeshCollisionSettings GetCollisionSettings() override;
	bool HasCollisionMesh() override;
	bool GetCollisionMesh(FRuntimeMeshCollisionData& CollisionData) override;
	bool IsThreadSafe() override;
};
