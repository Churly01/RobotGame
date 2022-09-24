// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueInteractionWidgetBlueprint.h"

void UTrueInteractionWidgetBlueprint::UpdateInteractionWidget(UInteractionComponent* InteractionComponent)
{
	OwningInteractionComponent = InteractionComponent;
	OnUpdateInteractionWidget();
}
