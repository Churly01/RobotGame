// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "../Player/RobotGameCharacter.h"
#include "../Widgets/TrueInteractionWidgetBlueprint.h"


UInteractionComponent::UInteractionComponent()
{
	SetComponentTickEnabled(false);
	/*
	Default values
	*/
	InteractionTime = 0.f;
	InteractionDistance = 200.f;
	InteractableName = FText::FromString("Interactable Object");
	InteractableAction = FText::FromString("Interact");
	bAllowMultipleInteractors = true;

	/*
	Widget Size on the screen
	*/
	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(400, 100);
	bDrawAtDesiredSize = true;

	/*Setting the component to active and hiding it at the start*/
	SetActive(true);
	SetHiddenInGame(true);
}



void UInteractionComponent::SetInteractableNameText(const FText& NewNameText)
{
	InteractableName = NewNameText;
	RefreshWidget();
}

void UInteractionComponent::SetInteractableActionText(const FText& NewActionText)
{
	InteractableAction = NewActionText;
	RefreshWidget();
}


bool UInteractionComponent::CanInteract(ARobotGameCharacter* Character) const
{
	// True if someone is interacting and its not allowed to have more than one interactor.
	bool bPlayerInteractingAlready = !bAllowMultipleInteractors && Interactors.Num() >= 1;

	// True if the component owner exists and the character exists as well.
	bool bIsValid = GetOwner() != nullptr && Character != nullptr;

	return !bPlayerInteractingAlready && IsActive() && bIsValid;

}

void UInteractionComponent::BeginFocus(class ARobotGameCharacter* Character)
{
	// If the interactable is not active, or does not have an owner or the character passed is null (Should not happen), then stop execution. 
	if (!(IsActive() && GetOwner() && Character)) {
		return;
	}


	OnBeginFocus.Broadcast(Character);
	SetHiddenInGame(false);
	//If we are not the server, then set render custom depth for each visual component.
	if (GetNetMode() != NM_DedicatedServer)
	{
		for (auto& VisualComp : GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass())) {
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(VisualComp);
			if (PrimComp)
			{
				PrimComp->SetRenderCustomDepth(true);
			}
		}
	}
	RefreshWidget();
}

void UInteractionComponent::EndFocus(class ARobotGameCharacter* Character)
{
	if (Character)
	{
		OnEndFocus.Broadcast(Character);
	}
	SetHiddenInGame(true);

	// We have to set render custom depth so the item stops outlining.
	if (GetNetMode() != NM_DedicatedServer)
	{
		for (auto& VisualComp : GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass())) {
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(VisualComp);
			if (PrimComp)
			{
				PrimComp->SetRenderCustomDepth(false);
			}
		}
	}
}

void UInteractionComponent::BeginInteract(class ARobotGameCharacter* Character)
{
	if (Character && CanInteract(Character))
	{
		Interactors.Add(Character);
		OnBeginInteract.Broadcast(Character);
	}
}

void UInteractionComponent::EndInteract(class ARobotGameCharacter* Character)
{
	Interactors.RemoveSingle(Character);
	//Using delegate. Whenever we want another component to do certain logic when a player ends an interaction we call this.
	OnEndInteract.Broadcast(Character);
}


void UInteractionComponent::Interact(class ARobotGameCharacter* Character)
{

	//Using delegate. Whenever we want another component to do certain logic when a player interacts we call this.
	OnInteract.Broadcast(Character);
}


void UInteractionComponent::RefreshWidget()
{
	// If the owner is not the server.
	if (!bHiddenInGame && GetOwner()->GetNetMode() != NM_DedicatedServer) {
		//Then, get the user widget and update it. 
		if (UTrueInteractionWidgetBlueprint* InteractionWidget = Cast<UTrueInteractionWidgetBlueprint>(GetUserWidgetObject()))
		{
			InteractionWidget->UpdateInteractionWidget(this);
		}
	}
}
void UInteractionComponent::Deactivate()
{
	Super::Deactivate();

	for (int32 i = Interactors.Num() - 1; i >= 0; i--) {
		if (ARobotGameCharacter* Interactor = Interactors[i]) {
			EndFocus(Interactor);
			EndInteract(Interactor);
		}
	}
	Interactors.Empty();
}
float UInteractionComponent::GetInteractPercentage()
{
	if (Interactors.IsValidIndex(0)) {
		// We null check the interactor and check if it is interacting with the component.
		ARobotGameCharacter* Interactor = Interactors[0];

		if (Interactor && Interactor->IsInteracting()) {
			// We calculate the percentage of time left of the interaction. 
			return 1.f - FMath::Abs(Interactor->GetRemainingInteractTime() / InteractionTime);
		}
	}
	return 0.f;
}
