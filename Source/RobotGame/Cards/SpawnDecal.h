// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnDecal.generated.h"

UCLASS()
class ROBOTGAME_API ASpawnDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnDecal();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly,Category = "Mesh")
	UStaticMeshComponent* DecalMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
