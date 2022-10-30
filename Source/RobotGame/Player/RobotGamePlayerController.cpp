// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotGamePlayerController.h"
#include "../Widgets/CardSlotWidget.h"
#include "../Cards/SpawnPlane.h"
#include "Engine/World.h"
#include "../Cards/SpawnDecal.h"
#include "GameFramework/Actor.h"
#include "RobotGameCharacter.h"

ARobotGamePlayerController::ARobotGamePlayerController()
{
	
}




void ARobotGamePlayerController::SetCardToSpawn(int32 SlotNum)
{
	if(SlotNum == 0)
	{
		CardsInfo.CardSlot = 0;
		CardsInfo.CardToSpawn = nullptr;
		return;
	}
	CardsInfo.CardSlot = SlotNum;
	CardsInfo.CardToSpawn = Slots[SlotNum-1]->CardOnDisplay;
}

void ARobotGamePlayerController::SpawnAt1()
{
	SetCardToSpawn(1);
}

void ARobotGamePlayerController::SpawnAt2()
{
	SetCardToSpawn(2);
}


void ARobotGamePlayerController::SpawnAt3()
{
	SetCardToSpawn(3);
}


void ARobotGamePlayerController::SpawnAt4()
{
	SetCardToSpawn(4);
}


void ARobotGamePlayerController::StopSpawning()
{
	
	if(CardsInfo.CardSlot !=0)
	{
		/*Should spawn a card. In that case, enqueue the last card and get the next card.*/
		SpawnCard(CardsInfo.CardToSpawn, CardsInfo.Loc);
		Enqueue(CardsInfo.CardToSpawn);
		Slots[CardsInfo.CardSlot-1]->SetCardOnDisplay(GetNextCard());

	}
	
	CardsInfo.CardSlot = 0; 
	CardsInfo.CardToSpawn = nullptr;
	if (ShownSpawnDecal) 
	{
		ShownSpawnDecal->Destroy();
	}
	ShownSpawnDecal = nullptr;

	
}

void ARobotGamePlayerController::Tick(float DeltaTime)
{
	if(CardsInfo.CardSlot !=0)
	{
		ProjectCardOnWorld(Slots[CardsInfo.CardSlot]);
	}
	
}



void ARobotGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARobotGamePlayerController, Deck);
}

UCard* ARobotGamePlayerController::Dequeue()
{
	UCard* card = Deck[0];
	Deck.RemoveAt(0);
	return card;
}

void ARobotGamePlayerController::Enqueue(UCard* Card)
{
	Deck.Add(Card);
}

void ARobotGamePlayerController::ServerSpawnNewCard_Implementation(UCard* CardToSpawn, FVector Loc)
{
	SpawnCard(CardToSpawn, Loc);
}

UCard* ARobotGamePlayerController::GetNextCard()
{
	return Dequeue();;
}



void ARobotGamePlayerController::SpawnCard(UCard* CardToSpawn, FVector Loc)
{
	if(HasAuthority())
	{
		UWorld* World = GetWorld();

		if (World && CardToSpawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("SPAWNING CARD"))
			FRotator rot;
			FActorSpawnParameters SpawnParams;
			SpawnParams.bNoFail = true; 
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			World->SpawnActor<ARobotGameCharacter>(CardToSpawn->SpawnableTemplate->GetDefaultObject()->GetClass(), Loc, rot, SpawnParams);
		}

	}
	else {
		ServerSpawnNewCard(CardToSpawn,Loc);
	}

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
	FRotator Rot = GetPawn()->GetActorRotation();
	Rot.Add(0, -90, 0);
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

	/*Setting appropriate position of the decal.*/
	CardsInfo.Loc = Loc;
	CardsInfo.Rot = Rot;

}


void ARobotGamePlayerController::SetDeck(TArray<UCard*> NewDeck)
{
	Deck = NewDeck;
}

void ARobotGamePlayerController::SetSlot(UCardSlotWidget* Slot)
{ 

	// TODO Check for null (Should not be null?) 
	
	if (Slot) 
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
	InputComponent->BindAction("Spawn1", IE_Pressed, this, &ARobotGamePlayerController::SpawnAt1);
	InputComponent->BindAction("Spawn1", IE_Pressed, this, &ARobotGamePlayerController::SpawnAt2);
	InputComponent->BindAction("Spawn1", IE_Pressed, this, &ARobotGamePlayerController::SpawnAt3);
	InputComponent->BindAction("Spawn1", IE_Pressed, this, &ARobotGamePlayerController::SpawnAt4);
	InputComponent->BindAction("Spawn1", IE_Released, this, &ARobotGamePlayerController::StopSpawning);


	// TODO Add bind to stop spawning if right clicked. 


}