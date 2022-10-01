// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structure.generated.h"

UCLASS()
class ROBOTGAME_API AStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStructure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StructureMesh;

	UPROPERTY(EditDefaultsOnly)
	class UFireWeaponComponent* Weapon;

	UPROPERTY(EditDefaultsOnly)
	class UAIPerceptionStimuliSourceComponent* AIPerceptionComponent;
public:	
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UStaticMeshComponent* Building;

	UPROPERTY(ReplicatedUsing = OnRepHealth)
	float Health;
	
	UPROPERTY(EditDefaultsOnly)
	float DamagePerHit;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnRepHealth();


};
