// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotGamePlayerController.h"
#include "../Widgets/CardSlotWidget.h"
#include "../Cards/SpawnPlane.h"
#include "Engine/World.h"
#include "../Cards/SpawnDecal.h"

ARobotGamePlayerController::ARobotGamePlayerController()
{
	
}




void ARobotGamePlayerController::IsSpawning1()
{
	bIsSpawning1 = true;
	bIsSpawning2 = false;
	bIsSpawning3 = false;
	bIsSpawning4 = false;
}

void ARobotGamePlayerController::IsSpawning2()
{
	bIsSpawning1 = false;
	bIsSpawning2 = true;
	bIsSpawning3 = false;
	bIsSpawning4 = false;
}

void ARobotGamePlayerController::IsSpawning3()
{
	bIsSpawning1 = false;
	bIsSpawning2 = false;
	bIsSpawning3 = true;
	bIsSpawning4 = false;

}

void ARobotGamePlayerController::IsSpawning4()
{
	bIsSpawning1 = false;
	bIsSpawning2 = false;
	bIsSpawning3 = false;
	bIsSpawning4 = true;
}

void ARobotGamePlayerController::StopSpawning()
{
	
	bIsSpawning1 = false;
	bIsSpawning2 = false;
	bIsSpawning3 = false;
	bIsSpawning4 = false;
	if (ShownSpawnDecal) {
		ShownSpawnDecal->Destroy();
	}
	ShownSpawnDecal->Destroy();
	ShownSpawnDecal = nullptr;

	// TODO Spawn card afterwards.
}

void ARobotGamePlayerController::Tick(float DeltaTime)
{
	if(bIsSpawning1)
	{
		ProjectCardOnWorld(Slots[0]);
	}
}

void ARobotGamePlayerController::ServerSpawnNewCard_Implementation(UCard* CardToSpawn)
{


// TODO Spawn Character
}

UCard* ARobotGamePlayerController::GetNextCard()
{
	UCard* Card;
	PlayingDeck.Dequeue(Card);
	return Card;
}


void ARobotGamePlayerController::ProjectCardOnWorld(UCardSlotWidget* Slot)
{
	// Make sure Slot is not null (Should never be)
	ensure(Slot);
	ensure(GetWorld());

	// Get Mouse Location in 3d space;
	FVector WorldLoc;
	FRotator WorldDirection;
	this->GetPlayerViewPoint(WorldLoc, WorldDirection);


	// Line trace to get whether we are spawning in our part of the arena.
	
	FHitResult HitResult;
	FVector Start = WorldLoc;
	FVector End = WorldLoc;
	End += WorldDirection.Vector() * 1000;
	
	
	// Make sure world is valid


	FCollisionQueryParams QueryParams;
	
	// Check for visibility. If its the spawn plane.
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

	// No hit
	if(!HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("A"))
		return;
	}
	// The hit actor is not the spawn plane.
	ASpawnPlane* SpawnPlane = Cast<ASpawnPlane>(HitResult.GetActor());
	if (!SpawnPlane)
	{
		return;
	}
	// Get Location and rotation
	FVector Loc = HitResult.ImpactPoint;
	FRotator Rot = GetPawn()->GetViewRotation();
	// Draw Card Spell or character. Place decal if its not placed.
	//Its spawned. Just move to new direction.
	if(ShownSpawnDecal)
	{
		UE_LOG(LogTemp, Warning, TEXT("decal exists"))
		// Change location and rotation. Set bsweep for smoother movement.
		ShownSpawnDecal->SetActorLocation(Loc);
		ShownSpawnDecal->SetActorRotation(Rot);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("create decal"))
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetPawn();
		// Change decal to be showing
		
		ShownSpawnDecal = GetWorld()->SpawnActor<ASpawnDecal>(Slot->CardOnDisplay->SpawnDecal->GetDefaultObject()->GetClass(), Loc, Rot, SpawnParams);
		
	}

	// TODO Queue back the card.

	// TODO Get Next Card for the slot from the queue

}




void ARobotGamePlayerController::CopyArrayIntoQueue()
{
	// We copy the array into the newly created queue.
	PlayingDeck.Empty();
	for (UCard* Card : Deck)
	{
		PlayingDeck.Enqueue(Card);
	}
}

void ARobotGamePlayerController::SetDeck(TArray<UCard*> NewDeck)
{
	Deck = NewDeck;
	CopyArrayIntoQueue();
}

void ARobotGamePlayerController::SetSlot(UCardSlotWidget* Slot)
{ 

	// TODO Check for null (Should not be null?) 
	
	if (Slot && Slot->CardOnDisplay) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ADDING ELEMENT TO ARRAY"))
		Slots.Add(Slot);
		UCard* NextCard = GetNextCard();
		Slot->CardOnDisplay = NextCard;
		Slot->OnUpdateCard(NextCard->Image);
	}



}

void ARobotGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Spawn1", IE_Pressed, this, &ARobotGamePlayerController::IsSpawning1);
	InputComponent->BindAction("Spawn1", IE_Released, this, &ARobotGamePlayerController::StopSpawning);


	// TODO Add bind to stop spawning if right clicked. 


}