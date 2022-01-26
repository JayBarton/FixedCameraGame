// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structs.h"
#include "FCObjectWatcher.h"
#include "FCSaveGame.generated.h"

/**
 * 
 */

//Just copying directly from instance right now, try to clean up later
class USoundBase;
class UAudioComponent;
UCLASS()
class FIXEDCAMERAGAME_API UFCSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString currentLevel;

	//Index of the player start and camera on entering a new level
	UPROPERTY(EditAnywhere, Category = "Player")
	int startIndex = 0;
	//Index of the camera on entering a new level. If this = -1, it is the same as the start index above
	UPROPERTY(EditAnywhere, Category = "Player")
		int cameraIndex = -1;
	UPROPERTY(EditAnywhere, Category = "Player")
		int equippedIndex = -1;
	UPROPERTY(EditAnywhere, Category = "Player")
		TArray<FItemStruct> playerInventory;
	UPROPERTY(EditAnywhere, Category = "Player")
		int32 playerHealth;

	UPROPERTY(EditAnywhere, Category = "Start")
		TArray<FItemStruct> containerInventory;

	//Use this to load objects that have data that can change, such as picked up items and locked doors
	UPROPERTY(EditAnywhere, Category = "Start")
		TMap<FString, FDataArray> savedObjects;

	//Use this to load enemy state
	UPROPERTY(EditAnywhere, Category = "Start")
		TMap<FString, FEnemyArray> savedEnemies;

	//Using this for one way locks
	UPROPERTY(EditAnywhere, Category = "Start")
		TMap<FString, FPendingLocks> pendingLocks;

	UPROPERTY(EditAnywhere, Category = "Start")
		TMap<FString, bool> levelMusicPlaying;

	UPROPERTY(EditAnywhere, Category = "Flags")
		TMap<FString, bool> flags;

	bool isPlayingMusic = false;

	UPROPERTY()
	UAudioComponent* music = nullptr;
	USoundBase* musicCue;
	
};
