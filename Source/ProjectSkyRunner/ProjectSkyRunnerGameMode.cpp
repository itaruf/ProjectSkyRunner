// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSkyRunnerGameMode.h"
#include "ProjectSkyRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectSkyRunnerGameMode::AProjectSkyRunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
