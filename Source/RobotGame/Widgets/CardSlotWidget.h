// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Player/RobotGamePlayerController.h"
#include "CardSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTGAME_API UCardSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//TODO CHange card on display.

	// Card to display on the slot.
	class UCard* CardOnDisplay;

	// Called to update the image when a card is changed
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateCard(UTexture2D* NewImage);

	UFUNCTION(BlueprintCallable)
	void SetCardOnDisplay(UCard* NewCardOnDisplay);
};
