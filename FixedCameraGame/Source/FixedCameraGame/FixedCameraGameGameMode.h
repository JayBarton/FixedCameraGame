// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FixedCameraGameGameMode.generated.h"

class UFCGameInstance;
class AFCObjectWatcher;
UCLASS(minimalapi)
class AFixedCameraGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFixedCameraGameGameMode();

	virtual void BeginPlay() override;

	void CheckObjects(UFCGameInstance* instance);

	void FindStart(UFCGameInstance* instance);

	void ChangeLevel(int index, FName levelName);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	AFCObjectWatcher* objectWatcher;

	FString currentLevel;
};



