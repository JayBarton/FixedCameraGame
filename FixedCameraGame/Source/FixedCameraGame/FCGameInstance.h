// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs.h"
#include "FCObjectWatcher.h"
#include "FCGameInstance.generated.h"


class USoundBase;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API UFCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//Index of the player start and camera on entering a new level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int startIndex = 0;
	//Index of the camera on entering a new level. If this = -1, it is the same as the start index above
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int cameraIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int equippedIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<FItemStruct> playerInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 playerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	TArray<FItemStruct> containerInventory;

	//Use this to load objects that have data that can change, such as picked up items and locked doors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	TMap<FString, FDataArray> savedObjects;

	//Use this to load enemy state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	TMap<FString, FEnemyArray> savedEnemies;

	//Using this for one way locks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	TMap<FString, FPendingLocks> pendingLocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags")
	TMap<FString, bool> flags;

	bool isPlayingMusic = false;

	UPROPERTY()
	UAudioComponent* music = nullptr;
	USoundBase* musicCue;


	UFUNCTION(BlueprintCallable)
	void PlayMusic(USoundBase* bgm, float volume = 1.0f, bool persist = true);

};
