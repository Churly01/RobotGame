// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Cards/Card.h"
#include "RobotGamePlayerController.generated.h"

/**
 * 
 */


// Create Delegate. Will be called whenever a card is spawn in order to delete the card and get the new one in the appropriate slot.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardUsed, int32, CardSlot);

UCLASS()
class ROBOTGAME_API ARobotGamePlayerController : public APlayerController
{
	GENERATED_BODY()

		ARobotGamePlayerController();

public:


	// DeckSize
	int32 DeckSize;
	// Deck of cards.
	TArray<UCard*> Deck;

	// Will start with the same cards as a deck. Used to get the next card in play. 
	TQueue<UCard*> PlayingDeck;

	// Slots where we have cards.
	TArray<class UCardSlotWidget*> Slots;

	
	class ASpawnDecal* ShownSpawnDecal;

	// Spawn Card in the appropriate location

	UFUNCTION(Server, Reliable)
	void ServerSpawnNewCard(UCard* CardToSpawn);

	// Gets the next card from the queue.
	UCard* GetNextCard();



	UFUNCTION(BlueprintCallable)
	// [CLIENT] Does the work of playing a card.
	void ProjectCardOnWorld(UCardSlotWidget* Slot);


	// TODO Add decal template for spell and character.
	UFUNCTION(BlueprintImplementableEvent)
	void DrawCharacterOnLocation(FVector DrawLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void DrawSpellOnLocation(FVector DrawLocation);

	// Restart queue.
	void CopyArrayIntoQueue();

	// Sets the deck to a new array. 
	UFUNCTION(BlueprintCallable)
	void SetDeck(TArray<UCard*> NewDeck);

	// To be called by the blueprint.
	UFUNCTION(BlueprintCallable)
	TArray<UCard*> GetDeck() {return Deck;}

	// Adds the slot to the array of slots. 
	UFUNCTION(BlueprintCallable)
	void SetSlot(class UCardSlotWidget* Slot);
	UFUNCTION(BlueprintCallable)	
		TArray<class UCardSlotWidget*> GetSlots()const { return Slots; }
};
