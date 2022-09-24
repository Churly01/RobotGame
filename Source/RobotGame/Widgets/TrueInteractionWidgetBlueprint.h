// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBlueprint.h"
#include "../Components/InteractionComponent.h"
#include "TrueInteractionWidgetBlueprint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTGAME_API UTrueInteractionWidgetBlueprint : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void UpdateInteractionWidget(UInteractionComponent* InteractionComponent);

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdateInteractionWidget();

	UPROPERTY(BlueprintReadOnly, Category = Interaction, meta = (ExponseOnSpawn))
		UInteractionComponent* OwningInteractionComponent;
	
};
