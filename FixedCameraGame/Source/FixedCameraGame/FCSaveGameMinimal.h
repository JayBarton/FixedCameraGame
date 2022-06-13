// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FCSaveGameMinimal.generated.h"

/**
 * 
 */
//Using this class to save a small amount of data that can be displayed in the UI without loading an entire file
UCLASS()
class FIXEDCAMERAGAME_API UFCSaveGameMinimal : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FString currentLevel;

	UPROPERTY()
	FString gameTime;
};
