// Fill out your copyright notice in the Description page of Project Settings.


#include "Structure.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "../Components/FireWeaponComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AStructure::AStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


	// Initialize components
	StructureMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = StructureMesh;

	Weapon = CreateDefaultSubobject<UFireWeaponComponent>("Weapon");

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AIPerception");

}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStructure::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStructure, Health)
}

// Called every frame
void AStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStructure::OnRepHealth()
{

}

