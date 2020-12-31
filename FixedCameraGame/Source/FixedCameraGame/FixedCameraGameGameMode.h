// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FixedCameraGameGameMode.generated.h"

UCLASS(minimalapi)
class AFixedCameraGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFixedCameraGameGameMode();

	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};



