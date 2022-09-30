// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeaponComponent.h"
#include "../Weapon/WeaponProjectile.h"
#include "../Player/RobotGameCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
UFireWeaponComponent::UFireWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Speed = 100.f;


}


// Called when the game starts
void UFireWeaponComponent::BeginPlay()
{

	Super::BeginPlay();

	// Set the PlayerPawn at the start of the game.
	PlayerPawn = Cast<ARobotGameCharacter>(GetOwner());
	
}


// Called every frame
void UFireWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFireWeaponComponent::SpawnProjectile(FRotator PlayerRotation)
{
	if(!CanFire())
	{
		return;
	}
	UWorld* World = GetWorld();
	if (ProjectileClass && PlayerPawn && World)
	{
		// We Spawn The Projectile
		FActorSpawnParameters SpawnParams;

		SpawnParams.bNoFail = true; // Has to spawn
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // Choose spawn type.

		FVector Loc;
		Loc = PlayerPawn->GetActorLocation();

		FVector ForwardVector;
		ForwardVector = PlayerPawn->GetActorForwardVector();

		if (PlayerPawn->GetWeaponMesh()) {
			Loc = PlayerPawn->GetWeaponMesh()->GetSocketLocation("ProjectileSocket");
		}
		SpawnParams.Owner = GetOwner();
		AWeaponProjectile* ProjectileToSpawn = World->SpawnActor<AWeaponProjectile>(ProjectileClass, Loc, PlayerRotation,SpawnParams);


		//Get Movement Component and give velocity.
		UProjectileMovementComponent* ProjectileMovement = ProjectileToSpawn->GetProjectileMovementComponent();

	}
}

bool UFireWeaponComponent::CanFire()
{
	bool IsFalling = PlayerPawn->GetMovementComponent()->IsFalling();
	return !IsFalling;
}

