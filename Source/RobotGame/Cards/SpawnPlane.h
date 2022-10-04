// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPlane.generated.h"

UCLASS()
class ROBOTGAME_API ASpawnPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* PlaneMesh;

	UPROPERTY(EditAnywhere, Category = "Multiplayer")
	FName Team;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
