// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotGamePlayerController.h"
#include "../Widgets/CardSlotWidget.h"

ARobotGamePlayerController::ARobotGamePlayerController()
{
	
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

	// Get Mouse Location in 3d space;
	FVector WorldLoc;
	FVector WorldDirection;
	DeprojectMousePositionToWorld(WorldLoc, WorldDirection);


	// Line trace to get whether we are spawning in our part of the arena.
	
	FHitResult HitResult;
	FVector Start = WorldLoc;
	FVector End = WorldLoc;
	End += WorldDirection * 1000;
	
	FCollisionQueryParams QueryParams;

	// Check for visibility. If its the spawn plane.
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);


	// Draw Card (If its a spell, draw decal to show area)
	if (Slot->CardOnDisplay->bIsCharacter)
	{

	}
	// If spell
	else
	{

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
		Slots.Add(Slot);
		UCard* NextCard = GetNextCard();
		Slot->CardOnDisplay = NextCard;
		Slot->OnUpdateCard(NextCard->Image);
	}

}
