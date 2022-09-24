// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTGAME_API UFireWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this component's properties
	UFireWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float Speed;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditDefaultsOnly, Category = "Time")
	float TimeBetweenShots;

	// ProjectileClass to spawn
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AWeaponProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(FRotator PlayerRotation);

	bool CanFire();

	class ARobotGameCharacter* PlayerPawn;
		
};
