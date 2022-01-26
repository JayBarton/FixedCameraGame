// Fill out your copyright notice in the Description page of Project Settings.


#include "FCGameInstance.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/AudioComponent.h" 
#include "FCSaveGame.h"


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
		UE_LOG(LogTemp, Warning, TEXT("what are you DOING %s"), *loadedGame->currentLevel);

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
		playerInventory = loadedGame->playerInventory;
		equippedIndex = loadedGame->equippedIndex;
		playerHealth = loadedGame->playerHealth;
		flags = loadedGame->flags;
		savedObjects = loadedGame->savedObjects;
		savedEnemies = loadedGame->savedEnemies;
		containerInventory = loadedGame->containerInventory;
		pendingLocks = loadedGame->pendingLocks;
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
