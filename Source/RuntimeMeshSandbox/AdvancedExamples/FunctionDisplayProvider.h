// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "FunctionDisplayProvider.generated.h"


UCLASS(HideCategories = Object, BlueprintType)
class RUNTIMEMESHSANDBOX_API UFunctionDisplayProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()
private:
	mutable FCriticalSection PropertySyncRoot;
	FBoxSphereBounds LocalBounds;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetDisplayMaterial, BlueprintSetter = SetDisplayMaterial)
	UMaterialInterface* DisplayMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetTime, BlueprintSetter = SetTime)
	float Time;



	float MinValue, MaxValue;
	float MinX, MaxX, MinY, MaxY;
	int32 PointsSX, PointsSY;
	float SizeX, SizeY, SizeZ;
public:
	UFunctionDisplayProvider();


	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetDisplayMaterial() const;
	UFUNCTION(BlueprintCallable)
	void SetDisplayMaterial(UMaterialInterface* InMaterial);

	UFUNCTION(BlueprintCallable)
	float GetTime() const;
	UFUNCTION(BlueprintCallable)
	void SetTime(float InTime);

protected:

	void Initialize() override;
	FBoxSphereBounds GetBounds() override;
	bool GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;
	bool IsThreadSafe() override;

private:
	void CalculateBounds();

	float CalculateHeightForPoint(float x, float y);
};
