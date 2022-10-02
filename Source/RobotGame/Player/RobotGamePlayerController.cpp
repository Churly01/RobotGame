// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotGamePlayerController.h"
#include "../Widgets/CardSlotWidget.h"

ARobotGamePlayerController::ARobotGamePlayerController()
{
	
}

void ARobotGamePlayerController::SpawnNewCard(UCard* CardToSpawn)
{
	// TODO Get Position To Spawn (Convert Mouse location to World Space)


	// TODO Line Trace to floor, appropriate channel


	// TODO Spawn Character
}

UCard* ARobotGamePlayerController::GetNextCard()
{
	UCard* Card;
	PlayingDeck.Dequeue(Card);
	return Card;
}


void ARobotGamePlayerController::UseCard(UCardSlotWidget* Slot)
{

	// Spawn Card

	// Queue back the card.

	// Get Next Card for the slot from the queue

	
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
	Slots.Add(Slot);
	UCard* NextCard = GetNextCard();
	Slot->CardOnDisplay = NextCard;
	Slot->OnUpdateCard(NextCard->Image);
}
