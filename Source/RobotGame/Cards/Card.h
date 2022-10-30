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
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
	//Template for the spell decal and the character decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnable", Meta = (ExposeOnSpawn = true))
	TSubclassOf<class ASpawnDecal> SpawnDecal;
	// Template used to spawn a character after placing the card. 
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	TSubclassOf<class ARobotGameCharacter> SpawnableTemplate;

	// Used to spawn a spell.
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	TSubclassOf<class ASpell> SpellTemplate;

	// If the template is a character or a spell
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable")
	bool bIsCharacter;

	// Image to be shown by the slot widget.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnable", Meta = (ExposeOnSpawn = true))
	UTexture2D* Image;
};
