// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotGameGameMode.h"
#include "RobotGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARobotGameGameMode::ARobotGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
