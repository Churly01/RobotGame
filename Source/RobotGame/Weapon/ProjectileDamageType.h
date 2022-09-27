// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "ProjectileDamageType.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTGAME_API UProjectileDamageType : public UDamageType
{
	GENERATED_BODY()
	
};


UCLASS()
class UExplosionDamage : public UProjectileDamageType
{
	GENERATED_BODY()
};

UCLASS()
class UNormalDamage : public UProjectileDamageType
{
	GENERATED_BODY()
};


UCLASS()
class UPiercingDamage : public UProjectileDamageType
{
	GENERATED_BODY()
};