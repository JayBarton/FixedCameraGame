// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs.h"
#include "FCObjectWatcher.h"
#include "FCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API UFCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//Index of the player start and camera on entering a new level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	int startIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
		int equippedIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	TArray<FItemStruct> playerInventory;

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


};
