// Copyright Epic Games, Inc. All Rights Reserved.

#include "FixedCameraGameGameMode.h"
#include "FixedCameraGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFixedCameraGameGameMode::AFixedCameraGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_FixedCameraPlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
