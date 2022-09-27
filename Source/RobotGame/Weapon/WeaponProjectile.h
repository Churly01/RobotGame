// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

UCLASS()
class ROBOTGAME_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	// ProjectileMesh
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ProjectileMesh;

	// Damage type to be done by the projectile
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> ProjectileDamageType;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement; }

	UPROPERTY(EditDefaultsOnly)
	float PointDamage;

	UPROPERTY(EditDefaultsOnly)
	float AreaDamage;

	UFUNCTION(BlueprintCallable)
	float GetPointDamage() const{return PointDamage;}


	UFUNCTION(BlueprintCallable)
	float GetAreaDamage() const { return AreaDamage; }

};
