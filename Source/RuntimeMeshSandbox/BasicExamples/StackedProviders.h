// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StackedProviders.generated.h"

UCLASS()
class RUNTIMEMESHSANDBOX_API AStackedProviders : public AActor
{
	GENERATED_BODY()
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"))
	class URuntimeMeshComponent* RuntimeMeshComponent;

public:	
	// Sets default values for this actor's properties
	AStackedProviders();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
