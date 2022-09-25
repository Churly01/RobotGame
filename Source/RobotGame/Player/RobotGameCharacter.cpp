// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Components/FireWeaponComponent.h"
#include "../Components/InteractionComponent.h"

//////////////////////////////////////////////////////////////////////////
// ARobotGameCharacter

ARobotGameCharacter::ARobotGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(),TEXT("head")); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));

	InteractionCheckDistance = 10000.f;
	
	WeaponComponent = CreateDefaultSubobject<UFireWeaponComponent>("FireWeaponComponent");
}



bool ARobotGameCharacter::IsInteracting()
{
	return GetWorldTimerManager().IsTimerActive(TimerHandle_Interact);
}

void ARobotGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PerformInteractionCheck();
}

void ARobotGameCharacter::FireWeapon(FRotator PlayerRotation)
{
	// Only Allow To fire after time is finished
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_Firing)) 
	{
		return;
	}
	// Set Timer
	if (WeaponComponent) 
	{
		GetWorldTimerManager().SetTimer(this->TimerHandle_Firing, this, &ARobotGameCharacter::Dummy, WeaponComponent->TimeBetweenShots, false);
	}

	if (HasAuthority()) 
	{
		WeaponComponent->SpawnProjectile(PlayerRotation);
	}
	else 
	{
		ServerFireWeapon(PlayerRotation);
	}
}

void ARobotGameCharacter::ServerFireWeapon_Implementation(FRotator PlayerRotation)
{
	FireWeapon(PlayerRotation);
}

void ARobotGameCharacter::Fire()
{
	FireWeapon(GetFollowCamera()->GetComponentRotation());
}

void ARobotGameCharacter::PerformInteractionCheck()
{
	if (this->GetController() == nullptr) {
		return;
	}

	// Update the time since last interaction
	InteractionData.LastInteractionTime = GetWorld()->GetTimeSeconds();


	// Eyes Position
	FVector EyesLoc;
	// Vector to where the eyes are looking at the moment
	FRotator EyesRot;
	// Getting from the controller the positions of the eyes and the direction to where they look 
	this->GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);


	// The start of the trace is the location of the eyes 
	FVector TraceStart = EyesLoc;
	// The end is the vector* the check distance of the character + the location of the eyes. 
	FVector TraceEnd = (EyesRot.Vector() * InteractionCheckDistance) + TraceStart;

	FHitResult TraceHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor())
		{
			// Check if the actor hit is interactable. That means, checking if it has an interaction component.
			UInteractionComponent* Interactable = Cast<UInteractionComponent>(TraceHit.GetActor()->GetComponentByClass(UInteractionComponent::StaticClass()));
			if (Interactable)
			{
				// We calculate the distance of the impact, taking the vector from the eyes to the object, then calculating the size
				float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				// Check whether we are already looking at the interactable. If its another object we check if its inside the range. 
				if (Interactable != GetInteractable() && Distance <= (Interactable->InteractionDistance))
				{	// Interactable found. 
					FoundNewInteractable(Interactable);
				}

				// If we did not find the interactable...
				else if (Distance > Interactable->InteractionDistance && GetInteractable())
				{
					CouldntFindInteractable();
				}
				// Return, interactable found but its the same we are already looking or outside of range. 
				return;
			}
		}
	}
	// At this point we did not find anything on the traceline. 
	CouldntFindInteractable();
}

void ARobotGameCharacter::CouldntFindInteractable()
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_Interact)) {
		GetWorldTimerManager().ClearTimer(TimerHandle_Interact);
	}

	UInteractionComponent* Interactable = GetInteractable();

	if (Interactable) {
		// We end the focus for the object and, in case the player was interacting we also end the interaction.
		Interactable->EndFocus(this);
		if (InteractionData.bInteractHeld) {
			EndInteract();
		}
	}

	InteractionData.ViewedInteractable = nullptr;
}

void ARobotGameCharacter::FoundNewInteractable(class UInteractionComponent* InteractableFound)
{
	// We end all previous interactions. 
	EndInteract();
	UE_LOG(LogTemp, Warning,TEXT("Found Interactable"))
	// Also, we end the focus if necessary from the old interactable
	UInteractionComponent* OldInteractable = this->GetInteractable();
	if (OldInteractable) {
		OldInteractable->EndFocus(this);
	}
	// Update new viewed interactable and begin focusing on it. 
	InteractionData.ViewedInteractable = InteractableFound;
	InteractableFound->BeginFocus(this);
}

void ARobotGameCharacter::BeginInteract()
{
	// If we are not the server, then we interact with the server function
	if (!HasAuthority()) {
		ServerBeginInteract();
	}


	if (HasAuthority()) {
		PerformInteractionCheck();
	}

	// Update if the interaction key is pressed or not. 
	InteractionData.bInteractHeld = true;

	// We get the interactable, then null check the interactable. 
	UInteractionComponent* Interactable = GetInteractable();

	// If there is an interatable, we start interacting. 
	if (Interactable) {
		// We start the interaction, passing the interacting actor to the interactable. 
		Interactable->BeginInteract(this);

		// If the interaction time is 0 or nearly 0, then interact straight away. 
		if (FMath::IsNearlyZero(Interactable->InteractionTime)) {
			Interact();
		}
		// Timed interact otherwise
		else {
			/*
			We start a timer with the interactable time, passing the 
			dle, the interact function to execute at the end
			and false to avoid looping.
			*/
			GetWorldTimerManager().SetTimer(this->TimerHandle_Interact, this, &ARobotGameCharacter::Interact, Interactable->InteractionTime, false);
		}
	}
}

void ARobotGameCharacter::EndInteract()
{
	if (!HasAuthority()) {
		ServerEndInteract();
	}

	InteractionData.bInteractHeld = false;


	// To end interaction we end the timer and then we get the interactable and end its interaction as well. 
	GetWorldTimerManager().ClearTimer(TimerHandle_Interact);
	UInteractionComponent* Interactable = GetInteractable();

	// If there is an interactable, we end interacting. 
	if (Interactable) {
		Interactable->EndInteract(this);
	}

}

void ARobotGameCharacter::ServerBeginInteract_Implementation()
{
	BeginInteract();
}

void ARobotGameCharacter::ServerEndInteract_Implementation()
{
	EndInteract();
}

void ARobotGameCharacter::Interact()
{
	// First, if we have a timed interaction, we end it 
	GetWorldTimerManager().ClearTimer(TimerHandle_Interact);
	// Null check the interactable and then interact with it. It will broadcast to the delegate to have custom functionalities. 
	UInteractionComponent* Interactable = this->GetInteractable();
	if (Interactable) {
		Interactable->Interact(this);
	}
}

float ARobotGameCharacter::GetRemainingInteractTime()
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle_Interact);
}

void ARobotGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ARobotGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ARobotGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ARobotGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ARobotGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ARobotGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}



//////////////////////////////////////////////////////////////////////////
// Input

void ARobotGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ARobotGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ARobotGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ARobotGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ARobotGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARobotGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARobotGameCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARobotGameCharacter::Fire);
}
