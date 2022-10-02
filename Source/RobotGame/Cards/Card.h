// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Card.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTGAME_API UCard : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	TSubclassOf<class ARobotGameCharacter> SpawnableTemplate;
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	TSubclassOf<class ASpell> SpellTemplate;
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	bool bIsCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "Spawnable", Meta = (ExposeOnSpawn = true))
	UTexture2D* Image;
};
