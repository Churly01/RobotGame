// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Cards/Card.h"
#include "Net/UnrealNetwork.h"
#include "Containers/Queue.h"
#include "RobotGamePlayerController.generated.h"

/**
 * 
 */


// Create Delegate. Will be called whenever a card is spawn in order to delete the card and get the new one in the appropriate slot.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardUsed, int32, CardSlot);


USTRUCT()
struct FSpawningCardInfo
{
	GENERATED_USTRUCT_BODY();
	UCard* CardToSpawn = nullptr;
	int32 CardSlot = 0;
	FVector Loc;
	FRotator Rot;
};

UCLASS()
class ROBOTGAME_API ARobotGamePlayerController : public APlayerController
{
	GENERATED_BODY()

		ARobotGamePlayerController();


private: 


	void SetCardToSpawn(int32 SlotNum);

	void SpawnAt1();
	void SpawnAt2();
	void SpawnAt3();
	void SpawnAt4();

	void StopSpawning();

public:
	FSpawningCardInfo CardsInfo;
	virtual void Tick(float DeltaTime) override;




	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UCard* Dequeue();

	void Enqueue(UCard* Card);



	// DeckSize
	int32 DeckSize;
	// Deck of cards.

	UPROPERTY(Replicated)
	TArray<UCard*> Deck;

	// Will start with the same cards as a deck. Used to get the next card in play.
	// Slots where we have cards.
	TArray<class UCardSlotWidget*> Slots;

	
	class ASpawnDecal* ShownSpawnDecal;

	// Spawn Card in the appropriate location

	UFUNCTION(Server, Reliable)
	void ServerSpawnNewCard(UCard* CardToSpawn, FVector Loc);

	// Gets the next card from the queue.

	UFUNCTION(BlueprintCallable)
	UCard* GetNextCard();


	void SpawnCard(UCard* CardToSpawn, FVector Loc);

	UFUNCTION(BlueprintCallable)
	// [CLIENT] Does the work of playing a card.
	void ProjectCardOnWorld(UCardSlotWidget* Slot);


	// TODO Add decal template for spell and character.
	UFUNCTION(BlueprintImplementableEvent)
	void DrawCharacterOnLocation(FVector DrawLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void DrawSpellOnLocation(FVector DrawLocation);


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

	bool isSpawningCard() {
		return CardsInfo.CardSlot != 0;
	}
	
	virtual void SetupInputComponent() override;
};
