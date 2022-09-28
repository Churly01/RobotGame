// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RobotGameCharacter.generated.h"

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()
	FInteractionData()
	{
		ViewedInteractable = nullptr;
		LastInteractionTime = 0.f;
		bInteractHeld = false;
	}

	// The current interactable the player is viewing if any
	UPROPERTY()
		class UInteractionComponent* ViewedInteractable;

	// Last time we checked for an interactable
	UPROPERTY()
		float LastInteractionTime;

	// Whether the player holding the interact button
	UPROPERTY()
		bool bInteractHeld;
};

UCLASS(config=Game)
class ARobotGameCharacter : public ACharacter
{
	GENERATED_BODY()


	void Dummy() {}

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

public:
	ARobotGameCharacter();


	void GetLifetimeReplicatedProps  (TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY()
	FInteractionData InteractionData;

	FORCEINLINE UInteractionComponent* GetInteractable() { return InteractionData.ViewedInteractable; }

	/** Timer Handle for the interactions. */
	FTimerHandle TimerHandle_Interact;


	// WeaponComponent
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class UFireWeaponComponent* WeaponComponent;

	FTimerHandle TimerHandle_Firing;
protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


	UPROPERTY(EditDefaultsOnly,  Category = "Weapon")
	FName FiringSocket;

public:

	// InteractionData
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckDistance;

	// To check whether or not is there an interactable in front 
	void PerformInteractionCheck();

	// No interactable found 
	void CouldntFindInteractable();

	// Interactable found
	void FoundNewInteractable(class UInteractionComponent* InteractableFound);


	void BeginInteract();
	void EndInteract();

	UFUNCTION(Server, Reliable)
	void ServerBeginInteract();

	UFUNCTION(Server, Reliable)
	void ServerEndInteract();

	void Interact();

	float GetRemainingInteractTime();
	bool IsInteracting();


	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Gets the FName for the firing socket. 
	FORCEINLINE FName GetFiringSocket() const { return FiringSocket;}

	// Gets the SkeletalMesh of the weapon
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	virtual void Tick(float DeltaTime) override;

	void FireWeapon(FRotator PlayerRotation);

	UFUNCTION(Server,Reliable)
	void ServerFireWeapon(FRotator PlayerRotation);

	void Fire();

	UPROPERTY(ReplicatedUsing = OnRepHealth)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	UFUNCTION()
	void OnRepHealth();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION(BlueprintCallable)
	float ModifyHealth(float Damage);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return Health; }

	UPROPERTY(ReplicatedUsing = OnRepKiller)
	ARobotGameCharacter* Killer;

	UFUNCTION()
	void OnRepKiller();
};

