// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionComponent.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, class ARobotGameCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, class ARobotGameCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, class ARobotGameCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, class ARobotGameCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, class ARobotGameCharacter*, Character);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTGAME_API UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
		float InteractionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
		float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
		FText InteractableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
		FText InteractableAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactions")
		bool bAllowMultipleInteractors;

	// Change the name text of the interactable item. 
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetInteractableNameText(const FText& NewNameText);

	// Change the name of the action of the interactable.
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetInteractableActionText(const FText& NewActionText);



	// Delegates
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnBeginInteract OnBeginInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnEndInteract OnEndInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnBeginFocus OnBeginFocus;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnEndFocus OnEndFocus;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnInteract OnInteract;


protected:
	virtual void Deactivate() override;

	bool CanInteract(ARobotGameCharacter* Character) const;


	// Array with all the players interacting with an item
	UPROPERTY()
		TArray<class ARobotGameCharacter*> Interactors;



public:


	// Refresh the widget whenever something changes on it. 
	void RefreshWidget();


	// Called when the player starts/stops looking at the interactable. (Hide/Show UI)
	void BeginFocus(class ARobotGameCharacter* Character);
	void EndFocus(class ARobotGameCharacter* Character);


	// Called when E is pressed or released. 
	void BeginInteract(class ARobotGameCharacter* Character);
	void EndInteract(class ARobotGameCharacter* Character);


	// When the interaction Ends
	void Interact(class ARobotGameCharacter* Character);

	UFUNCTION(BlueprintPure, Category = "Interaction")
		float GetInteractPercentage();

};
