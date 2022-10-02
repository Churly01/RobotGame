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

	class UCard* CardOnDisplay;

	class ARobotGamePlayerController* Owner;

	UFUNCTION(BlueprintCallable)
	void SetPlayerController(ARobotGamePlayerController* PC)
	{
		Owner = PC;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateCard(UTexture2D* NewImage);

};
