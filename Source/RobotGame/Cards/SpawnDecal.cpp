// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnDecal.h"

// Sets default values
ASpawnDecal::ASpawnDecal()
{
	
	DecalMesh = CreateDefaultSubobject<UStaticMeshComponent>("DecalMesh");
	RootComponent = DecalMesh;
}

// Called when the game starts or when spawned
void ASpawnDecal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

