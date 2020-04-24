// Fill out your copyright notice in the Description page of Project Settings.

#include "StackedProviders.h"
#include "RuntimeMeshComponent.h"
#include "Providers/RuntimeMeshProviderPlane.h"
#include "Providers/RuntimeMeshProviderCollision.h"


// Sets default values
AStackedProviders::AStackedProviders()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RuntimeMeshComponent = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("RuntimeMeshComponent"));
	RootComponent = RuntimeMeshComponent;
}

// Called when the game starts or when spawned
void AStackedProviders::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStackedProviders::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	URuntimeMeshProviderPlane* PlaneProvider = NewObject<URuntimeMeshProviderPlane>(this, TEXT("RuntimeMeshProvider-Plane"));
	URuntimeMeshProviderCollision* CollisionProvider = NewObject<URuntimeMeshProviderCollision>(this, TEXT("RuntimeMeshProvider-Collision"));
	if (PlaneProvider && CollisionProvider)
	{
		CollisionProvider->SetChildProvider(PlaneProvider);
		CollisionProvider->SetRenderableLODForCollision(2);
		CollisionProvider->SetRenderableSectionAffectsCollision(0, true);
		FRuntimeMeshCollisionSettings CollisionSettings;
		CollisionSettings.bUseComplexAsSimple = true;
		CollisionSettings.bUseAsyncCooking = true;
		CollisionProvider->SetCollisionSettings(CollisionSettings);
		RuntimeMeshComponent->Initialize(CollisionProvider);
	}
}

// Called every frame (unused, and set to tick to false)
void AStackedProviders::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

