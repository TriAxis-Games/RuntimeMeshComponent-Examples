// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshProvider.h"
#include "FunctionDisplayProvider.generated.h"

/**
 * 
 */
class RUNTIMEMESHSANDBOX_API FFunctionDisplayProviderProxy : public FRuntimeMeshProviderProxy
{
public:
	TWeakObjectPtr<UMaterialInterface> Material;
	float function(float x, float y);
	float minvalue, maxvalue;
	float minx, maxx, miny, maxy;
	int32 pointsx, pointsy;
	float sizex, sizey, sizez;

	float time;

	FFunctionDisplayProviderProxy(TWeakObjectPtr<URuntimeMeshProvider> InParent);
	~FFunctionDisplayProviderProxy();

	void UpdateProxyParameters(URuntimeMeshProvider* ParentProvider, bool bIsInitialSetup) override;

	virtual void Initialize() override;

	virtual bool GetSectionMeshForLOD(int32 LODIndex, int32 SectionId, FRuntimeMeshRenderableMeshData& MeshData) override;

	virtual FBoxSphereBounds GetBounds() override;

	bool IsThreadSafe() const override { return true; }
};

UCLASS(HideCategories = Object, BlueprintType)
class RUNTIMEMESHSANDBOX_API UFunctionDisplayProvider : public URuntimeMeshProvider
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	float time;

	UFunctionDisplayProvider();

	UFUNCTION(BlueprintCallable)
		void SetTime(float time);

protected:
	virtual FRuntimeMeshProviderProxyRef GetProxy() override;
	
};
