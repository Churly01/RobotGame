// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPlane.h"

// Sets default values
ASpawnPlane::ASpawnPlane()
{
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = PlaneMesh;
}

// Called when the game starts or when spawned
void ASpawnPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

