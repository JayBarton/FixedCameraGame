// Fill out your copyright notice in the Description page of Project Settings.


#include "FCGameInstance.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetStringLibrary.h"
#include "Components/AudioComponent.h" 
#include "FCSaveGame.h"
#include "FCSaveGameMinimal.h"

void UFCGameInstance::PlayMusic(USoundBase* bgm, float volume, bool persist)
{
	//music = UGameplayStatics::SpawnSound2D(GetWorld(), bgm, volume, 1.0f, 0.0f, (USoundConcurrency*)nullptr, persist);
	isPlayingMusic = true;
	music = UGameplayStatics::CreateSound2D(GetWorld(), bgm, 0.5f, 1.0f, 0.0f, (USoundConcurrency*)nullptr, true);
	music->FadeIn(1.0f);
}

void UFCGameInstance::StopMusic(float transitionTime)
{
	music->FadeOut(transitionTime, 0.0f);
	isPlayingMusic = false;
}

void UFCGameInstance::LoadGame(int slot)
{
	FString slotName = "slot" + FString::FromInt(slot);
	if (UFCSaveGame* loadedGame = Cast<UFCSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 1)))
	{
		currentLevel = loadedGame->currentLevel;

		/*FString level = "MainHallway";
		for (auto& Elem : loadedGame->savedEnemies)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Elem.Key);

		}
		loadedGame->savedEnemies[level];
		if (loadedGame->savedEnemies.Num() > 0 && loadedGame->savedEnemies[level].data.Num() > 0)
		{
			if (loadedGame->savedEnemies[level].data[0].canSpawn)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy can spawn"))
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy cannot spawn"))

			}
		}*/

		startIndex = loadedGame->startIndex;
		cameraIndex = loadedGame->cameraIndex;
		if (cameraIndex == -1)
		{
			cameraIndex = 0;
		}
		playerInventory = loadedGame->playerInventory;
		equippedIndex = loadedGame->equippedIndex;
		playerHealth = loadedGame->playerHealth;
		flags = loadedGame->flags;
		savedObjects = loadedGame->savedObjects;
		savedEnemies = loadedGame->savedEnemies;
		containerInventory = loadedGame->containerInventory;
		pendingLocks = loadedGame->pendingLocks;
		gameTime = loadedGame->gameTime;
		numberOfSaves = loadedGame->numberOfSaves;
		enemyKills = loadedGame->enemyKills;

		UE_LOG(LogTemp, Warning, TEXT("load time %f"), loadedGame->gameTime);
		UE_LOG(LogTemp, Warning, TEXT("saves %i"), numberOfSaves);
		//not sure about this.
		/*if (saveGamelevelMusicPlaying.Contains(currentLevel))
		{
			saveGamelevelMusicPlaying[currentLevel] = objectWatcher->instanceMusic;
		}
		else
		{
			saveGamelevelMusicPlaying.Add(currentLevel, objectWatcher->instanceMusic);
		}

		if (container)
		{
			saveGamecontainerInventory = container->Inventory->inventory;
		}*/

		UE_LOG(LogTemp, Warning, TEXT("slot %s"), *slotName);
		UE_LOG(LogTemp, Warning, TEXT("LOADED current level %s"), *currentLevel);
		UGameplayStatics::OpenLevel(GetWorld(), FName(*currentLevel));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no save in this slot %s"), *slotName);
	}
}

FMinimalSaveStruct UFCGameInstance::LoadMinimal(int slot)
{
	FMinimalSaveStruct saveStruct;
	FString slotName = "slotM" + FString::FromInt(slot);
	if (UFCSaveGameMinimal* loadedGame = Cast<UFCSaveGameMinimal>(UGameplayStatics::LoadGameFromSlot(slotName, 1)))
	{
		saveStruct.level = FName::NameToDisplayString(loadedGame->currentLevel, false);
		saveStruct.loaded = true;
		saveStruct.playTime = loadedGame->gameTime;
		return saveStruct;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no save in this slot %s"), *slotName);
	}
	return saveStruct;

}

FString UFCGameInstance::GetTime()
{
	int seconds = fmod(gameTime, 60);
	int minutes = fmod(gameTime, 3600) / 60;
	UE_LOG(LogTemp, Warning, TEXT("%i"), minutes);

	int hours = gameTime / 3600;
	return FString::Printf(TEXT("%02d:%02d:%02d"), hours, minutes, seconds);
}

void UFCGameInstance::Reset()
{
	currentLevel = "";

	numberOfSaves = 0;
	enemyKills = 0;

	startIndex = 0;
	cameraIndex = -1;
	equippedIndex = -1;
	playerInventory.Empty();
	playerHealth = 100;

	containerInventory.Empty();

	gameTime = 0.0f;

	savedObjects.Empty();

	savedEnemies.Empty();

	pendingLocks.Empty();

	levelMusicPlaying.Empty();

	for (auto& flag : flags)
	{
		flag.Value = false;
	}

	isPlayingMusic = false;

	music = nullptr;
	musicCue = nullptr;

	entranceSoundIndex = -1;
}
