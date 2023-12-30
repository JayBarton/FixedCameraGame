// Copyright Epic Games, Inc. All Rights Reserved.

#include "FixedCameraGameGameMode.h"
#include "FixedCameraGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FCGameInstance.h"
#include "FCPlayer.h"
#include "FCInventoryComponent.h"
#include "FC_PlayerStart.h"
#include "FCPlayerCamera.h"
#include "FCObjectWatcher.h"
#include "FCContainer.h"
#include "FCInfoTextWidget.h"
#include "FCLockComponent.h"
#include "FCMultiLockComponent.h"
#include "FCPuzzleInteractable.h"
#include "FCSwitchInteractable.h"
#include "FCSwitchComponent.h"
#include "FCExit.h"
#include "FCEnemy.h"
#include "FCEnemy_Normal.h"
#include "FCEnemy_Patrol.h"
#include "FCEnemySpawn.h"
#include "FCPickup.h"
#include "FCSwitchComponent.h"
#include "FCSaveGame.h"
#include "FCSaveGameMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine/Engine.h"
#include "Components/AudioComponent.h"


AFixedCameraGameGameMode::AFixedCameraGameGameMode()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bCanEverTick = true;

}

void AFixedCameraGameGameMode::BeginPlay()
{

	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFCObjectWatcher::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		objectWatcher = Cast<AFCObjectWatcher>(FoundActors[0]);
	}

	if (instance)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFCContainer::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			container = Cast<AFCContainer>(FoundActors[0]);
			if (instance->containerInventory.Num() == container->Inventory->inventory.Num())
			{
				container->Inventory->inventory = instance->containerInventory;
			}
		}
		CheckInstance(instance);

		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			//Need to check this in case this begin play is called after the player inventory beginplay
			if (instance->playerInventory.Num() > 0)
			{
				pc->Inventory->inventory = instance->playerInventory;
			}
			pc->equipped = instance->equippedIndex;
			pc->currentHealth = instance->playerHealth;

			pc->Init();
		}

		FindStart(instance);
		levelFadeIn = true;
		playerCamera->SetMaterial(1.0f);

		if (instance->entranceSoundIndex >= 0 && enterSound[instance->entranceSoundIndex])
		{
			UGameplayStatics::PlaySound2D(GetWorld(), enterSound[instance->entranceSoundIndex]);
			instance->entranceSoundIndex = -1;
		}
		
	}
}

void AFixedCameraGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (levelFadeOut)
	{
		transitionTimer += DeltaTime;
		//update camera
		playerCamera->UpdateMaterial(transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			MoveToLevel(nextLevel);
		}
	}
	else if (levelFadeIn)
	{
		transitionTimer += DeltaTime;
		playerCamera->UpdateMaterial(1.0f - transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			transitionTimer = 0.0f;
			levelFadeIn = false;
		}
	}

	//make sure the camera is still updating when the game is paused
	//To allow changing cameras while paused
	if(UGameplayStatics::IsGamePaused(GetWorld()))
	{
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		pc->UpdateCameraManager(DeltaTime);
	}
	else
	{
		if (!fakePause)
		{
			gameTime += DeltaTime;
		//	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetTime())
		}
	}
}

void AFixedCameraGameGameMode::CheckInstance(UFCGameInstance* instance)
{
	currentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	instance->currentLevel = currentLevel;

	currentIndex = instance->startIndex;
	if (instance->cameraIndex == -1)
	{
		instance->cameraIndex = currentIndex;
	}
	currentCameraIndex = instance->cameraIndex;

	if (objectWatcher)
	{
		if (instance->savedObjects.Contains(currentLevel))
		{
			CheckObjects(instance);
			CheckEnemies(instance);
		}
		else
		{
			SpawnEnemies(instance, true);
			//check active objects
			for (int i = 0; i < objectWatcher->objects.data.Num(); i++)
			{
				auto object = objectWatcher->objects.data[i];
				if (!object.active)
				{
					if (auto pickup = Cast<AFCPickup>(object.actor))
					{
						pickup->active = false;
						pickup->MeshComp->SetHiddenInGame(true, true);
						pickup->SetActorEnableCollision(false);
					}
					else if (auto interactable = Cast<AFCInteractable>(object.actor))
					{
						interactable->active = false;
					}
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("first time in %s"), *currentLevel);
		}
		objectWatcher->instanceMusic = objectWatcher->playingMusic;

		HandlePendingLocks(instance);

		HandleMusic(instance);

		gameTime = instance->gameTime;
		enemyKills = instance->enemyKills;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no object watcher"));
	}
}

void AFixedCameraGameGameMode::HandleMusic(UFCGameInstance* instance)
{
	if (instance->levelMusicPlaying.Contains(currentLevel))
	{
		objectWatcher->playingMusic = instance->levelMusicPlaying[currentLevel];
	}
	if (!instance->isPlayingMusic)
	{
		if (objectWatcher->playingMusic == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("FFF"));
		}
		if (objectWatcher->levelMusic && objectWatcher->playingMusic)
		{
			PlayMusic(instance);
		}
	}
}

void AFixedCameraGameGameMode::PlayMusic(UFCGameInstance* instance)
{
	instance->PlayMusic(objectWatcher->levelMusic, 0.5f);
}

void AFixedCameraGameGameMode::StartNewMusic(bool playOnce)
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PlayMusic(instance);
	UE_LOG(LogTemp, Warning, TEXT("m1"));
	objectWatcher->playingMusic = !playOnce;

}

void AFixedCameraGameGameMode::StopMusic()
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	instance->StopMusic(transitionTime);
	objectWatcher->playingMusic = false;
}

void AFixedCameraGameGameMode::CheckObjects(UFCGameInstance* instance)
{
	//if they are the same size we must have saved the data at some point
	if (objectWatcher->objects.data.Num() == instance->savedObjects[currentLevel].data.Num())
	{
		for (int i = 0; i < objectWatcher->objects.data.Num(); i++)
		{
			objectWatcher->objects.data[i].spawn = instance->savedObjects[currentLevel].data[i].spawn;
			objectWatcher->objects.data[i].locked = instance->savedObjects[currentLevel].data[i].locked;
			objectWatcher->objects.data[i].active = instance->savedObjects[currentLevel].data[i].active;
			objectWatcher->objects.data[i].switched = instance->savedObjects[currentLevel].data[i].switched;
		}

		for (int i = 0; i < objectWatcher->objects.data.Num(); i++)
		{
			auto object = objectWatcher->objects.data[i];
			if (!object.spawn)
			{
				object.actor->Destroy();
			}
			else
			{
				if (!object.locked)
				{
					if (auto lock = object.actor->FindComponentByClass(UFCLockComponent::StaticClass()))
					{
						UE_LOG(LogTemp, Warning, TEXT("L1"));
						lock->DestroyComponent();
					}
					else if (auto multiLock = object.actor->FindComponentByClass(UFCMultiLockComponent::StaticClass()))
					{
						multiLock->DestroyComponent();
					}
				}
			}
			if (!object.active)
			{
				UE_LOG(LogTemp, Warning, TEXT("L2"));

				if (auto pickup = Cast<AFCPickup>(object.actor))
				{
					UE_LOG(LogTemp, Warning, TEXT("A2"));
					pickup->active = false;
					pickup->MeshComp->SetHiddenInGame(true, true);
					pickup->SetActorEnableCollision(false);
				}
				else if (auto interactable = Cast<AFCInteractable>(object.actor))
				{
					interactable->active = false;
				}
			}
			if (object.switched)
			{
				UE_LOG(LogTemp, Warning, TEXT("L3"));
				if (auto switchComponent = Cast<UFCSwitchComponent>(object.actor->FindComponentByClass(UFCSwitchComponent::StaticClass())))
				{
					UE_LOG(LogTemp, Warning, TEXT("switch here"));

					switchComponent->switchState = true;
					switchComponent->SwitchOn.Broadcast();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error"));
	}
}

void AFixedCameraGameGameMode::CheckEnemies(UFCGameInstance* instance)
{
	if (objectWatcher->enemies.data.Num() == instance->savedEnemies[currentLevel].data.Num())
	{
		for (int i = 0; i < objectWatcher->enemies.data.Num(); i++)
		{
			objectWatcher->enemies.data[i].spawn = instance->savedEnemies[currentLevel].data[i].spawn;
			objectWatcher->enemies.data[i].canSpawn = instance->savedEnemies[currentLevel].data[i].canSpawn;
			objectWatcher->enemies.data[i].alive = instance->savedEnemies[currentLevel].data[i].alive;
			objectWatcher->enemies.data[i].spawnIn = instance->savedEnemies[currentLevel].data[i].spawnIn;
			objectWatcher->enemies.data[i].reviveCount = instance->savedEnemies[currentLevel].data[i].reviveCount;
			objectWatcher->enemies.data[i].reviveTime = instance->savedEnemies[currentLevel].data[i].reviveTime;
			objectWatcher->enemies.data[i].transform = instance->savedEnemies[currentLevel].data[i].transform;
		}
		UE_LOG(LogTemp, Warning, TEXT("CHECK 1"));

		SpawnEnemies(instance);
	}
}

void AFixedCameraGameGameMode::SpawnEnemies(UFCGameInstance* instance, bool firstTime)
{
	for (int i = 0; i < objectWatcher->enemies.data.Num(); i++)
	{
		FEnemiesToWatch& enemy = objectWatcher->enemies.data[i];

		if (instance->flags.Contains(enemy.deSpawnFlag))
		{
			enemy.spawn = !instance->flags[enemy.deSpawnFlag];
		}
		else
		{
			if (enemy.spawnFlag == "")
			{
				enemy.spawn = enemy.canSpawn;
				//	canSpawn = true;
			}
			else
			{
				if (enemy.canSpawn)
				{
					if (instance->flags.Contains(enemy.spawnFlag))
					{
						enemy.spawn = instance->flags[enemy.spawnFlag];
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("you know"));
						//Don't know that this else is needed, as it's not like I'll be creating flags on the fly
						//keeping it around for now until I finalize this flag idea
						enemy.spawn = false;
					}
				}
				else
				{
					enemy.spawn = false;
				}
			}
		}

		if (enemy.spawn)
		{
			//enemy.spawn = true;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			enemy.enemy = GetWorld()->SpawnActor<AFCEnemy>(enemy.spawnActor->enemyType, enemy.transform, SpawnParams);
			enemy.spawnActor->spawnedEnemy = enemy.enemy;
			if (auto patrolEnemy = Cast<AFCEnemy_Patrol>(enemy.enemy))
			{
				patrolEnemy->InitPath(enemy.spawnActor->patrolPoints);
			}
			else if (auto normalEnemy = Cast<AFCEnemy_Normal>(enemy.enemy))
			{
				if (!enemy.alive)
				{
					enemy.enemy->SetActorTransform(enemy.transform);
					int32 currentReviveTime;
					if (firstTime)
					{
						currentReviveTime = enemy.spawnActor->reviveCount;
						UE_LOG(LogTemp, Warning, TEXT("Hey man"));
					}
					else
					{
						currentReviveTime = enemy.reviveTime;
					}

					normalEnemy->StartDead(currentReviveTime, enemy.reviveCount);
				}
			}
			if (enemy.alive && enemy.spawnIn)
			{
				enemy.enemy->SetUpSpawnIn();
			}
		}
	}
}

void AFixedCameraGameGameMode::HandlePendingLocks(UFCGameInstance* instance)
{
	if (instance->pendingLocks.Contains(currentLevel))
	{
		for (int i = 0; i < objectWatcher->objects.data.Num(); i++)
		{
			auto object = objectWatcher->objects.data[i];
			if (object.locked)
			{
				if (auto door = Cast<AFCExit>(object.actor))
				{
					for (int c = 0; c < instance->pendingLocks[currentLevel].data.Num(); c++)
					{
						if (door->currentIndex == instance->pendingLocks[currentLevel].data[c])
						{
							if (auto lock = object.actor->FindComponentByClass(UFCLockComponent::StaticClass()))
							{
								instance->pendingLocks[currentLevel].data.RemoveAt(c);
								c--;
								lock->DestroyComponent();
							}
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("one way lock attached to non-exit"));
				}
			}
		}
		if (instance->pendingLocks[currentLevel].data.Num() == 0)
		{
			instance->pendingLocks.Remove(currentLevel);
		}
	}
}

void AFixedCameraGameGameMode::FindStart(UFCGameInstance* instance)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		playerCamera = Cast<AFCPlayerCamera>(FoundActors[i]);
		if (playerCamera)
		{
			if (playerCamera->startIndex == instance->cameraIndex)
			{
				APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (playerController)
				{
					FViewTargetTransitionParams transitionParams;
					playerController->SetViewTarget(playerCamera, transitionParams);
					return;
				}
			}
		}
	}
	//if the start camera is not properly set
	UE_LOG(LogTemp, Warning, TEXT("start camera not found, view not set"))
}

void AFixedCameraGameGameMode::ChangeLevel(int index, int cameraIndex, FName levelName, bool continueMusic)
{
	//Want to play the door sound effect. Can either be done here or in a separate funciton called from FCExit
	//Benefit of a separate function is it would make passing a variable for door sounds a bit cleaner
	//Easiest thing would either be in FCExit or in the if newLevel condition
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			if (objectWatcher)
			{
				if (newLevel)
				{
					instance->startIndex = index;
					if (cameraIndex == -1)
					{
						instance->cameraIndex = index;
					}
					else
					{
						instance->cameraIndex = cameraIndex;
					}
					instance->playerInventory = pc->Inventory->inventory;
					instance->equippedIndex = pc->equipped;
					instance->playerHealth = pc->currentHealth;
					objectWatcher->UpdateObjects();
					UGameplayStatics::SetGamePaused(GetWorld(), true);

				}
				for (int i = 0; i < objectWatcher->pendingFlags.Num(); i++)
				{
					auto flag = objectWatcher->pendingFlags[i];
					if (instance->flags.Contains(flag))
					{
						instance->flags[flag] = true;
					}
				}
				if (instance->savedObjects.Contains(currentLevel))
				{
					instance->savedObjects[currentLevel].data = objectWatcher->objects.data;
					instance->savedEnemies[currentLevel].data = objectWatcher->enemies.data;
				}
				else
				{
					instance->savedObjects.Add(currentLevel, objectWatcher->objects);
					instance->savedEnemies.Add(currentLevel, objectWatcher->enemies);
				}
				if (instance->levelMusicPlaying.Contains(currentLevel))
				{
					instance->levelMusicPlaying[currentLevel] = objectWatcher->instanceMusic;
				}
				else
				{
					instance->levelMusicPlaying.Add(currentLevel, objectWatcher->instanceMusic);
				}
			}

			if (container)
			{
				instance->containerInventory = container->Inventory->inventory;
			}

			levelFadeOut = true;
			nextLevel = levelName;
			playerCamera = Cast<AFCPlayerCamera>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewTarget());
			UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			instance->gameTime = gameTime;
			instance->enemyKills = enemyKills;
			if (!continueMusic && instance->isPlayingMusic)
			{
				instance->StopMusic(transitionTime);
			}
		}
	}
}

void AFixedCameraGameGameMode::MoveToLevel(FName levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), levelName);
}

void AFixedCameraGameGameMode::EndGame(bool dead)
{
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	instance->equippedIndex = pc->equipped;
	//this works differently now, figure out how to refactor later
	//I still want the fade out behavior, and updating the equip index, but don't need any object watcher stuff
	newLevel = false;
	playerCamera = Cast<AFCPlayerCamera>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewTarget());
	
	//Need a new function for change level. I don't need anything except for the fade out when dead, and I just need to update some instance data(not all) on complete
	if (dead)
	{
		playerCamera->dynamicMaterial->SetVectorParameterValue("Color", FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		ChangeLevel(currentIndex, currentCameraIndex, "GameOverScene", false);
	}
	else
	{
		//will be results screen
		ChangeLevel(currentIndex, currentCameraIndex, "EndGameResults", false);
	}
}

void AFixedCameraGameGameMode::SetPendingLock(FString levelName, int32 index)
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		if (instance->pendingLocks.Contains(levelName))
		{
			instance->pendingLocks[levelName].data.Add(index);
		}
		else
		{
			FPendingLocks newLock;
			newLock.data.Add(index);
			instance->pendingLocks.Add(levelName, newLock);
		}

	}
}

void AFixedCameraGameGameMode::StopEnemies(bool pauseAnimation, bool hideEnemy)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFCEnemy::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		auto enemy = Cast<AFCEnemy>(FoundActors[i]);
		if (!enemy->dead)
		{
			if (AController* AI = enemy->GetController())
			{
				AI->StopMovement();
			}
			enemy->PawnSensingComp->SetSensingUpdatesEnabled(false);
			enemy->PauseSounds();
			enemy->SetActorTickEnabled(false);
			enemy->GetMesh()->bPauseAnims = pauseAnimation;
			enemy->GetMesh()->SetHiddenInGame(hideEnemy, true);
		}
	}
}

void AFixedCameraGameGameMode::ResumeEnemies()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFCEnemy::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		auto enemy = Cast<AFCEnemy>(FoundActors[i]);
		if (!enemy->dead)
		{
			enemy->PawnSensingComp->SetSensingUpdatesEnabled(true);
			enemy->SetActorTickEnabled(true);
			enemy->GetMesh()->bPauseAnims = false;
			enemy->GetMesh()->SetHiddenInGame(false, true);
			enemy->ResetNoiseTimer();
		}
	}
}

void AFixedCameraGameGameMode::SaveGame(int slot, int token, int playerStart)
{
	UFCSaveGame* saveGameInstance = Cast<UFCSaveGame>(UGameplayStatics::CreateSaveGameObject(UFCSaveGame::StaticClass()));
	UFCSaveGameMinimal* saveGameMinimalInstance = Cast<UFCSaveGameMinimal>(UGameplayStatics::CreateSaveGameObject(UFCSaveGameMinimal::StaticClass()));
	if (saveGameInstance && saveGameMinimalInstance)
	{
		saveGameInstance->currentLevel = currentLevel;
		saveGameMinimalInstance->currentLevel = currentLevel;
		int seconds = fmod(gameTime, 60);
		int minutes = fmod(gameTime, 3600) / 60;
		int hours = gameTime / 3600;
		saveGameMinimalInstance->gameTime = GetTime();

		auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (instance)
		{
			auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (pc)
			{
				if (objectWatcher)
				{
					auto Inventory = pc->Inventory;
					FItemStruct& slot = Inventory->inventory[token];
					slot.amount--;
					if (slot.amount <= 0)
					{
						slot = FItemStruct();
					}
					auto currentCamera = Cast<AFCPlayerCamera>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewTarget());
					saveGameInstance->startIndex = playerStart;
					saveGameInstance->cameraIndex = currentCamera->startIndex;
					saveGameInstance->playerInventory = pc->Inventory->inventory;
					saveGameInstance->equippedIndex = pc->equipped;
					saveGameInstance->playerHealth = pc->currentHealth;

					saveGameInstance->gameTime = gameTime;
					saveGameInstance->enemyKills = enemyKills;
					UE_LOG(LogTemp, Warning, TEXT("save time %f"), saveGameInstance->gameTime);

					objectWatcher->UpdateObjects();
					for (int i = 0; i < objectWatcher->pendingFlags.Num(); i++)
					{
						auto flag = objectWatcher->pendingFlags[i];
						if (instance->flags.Contains(flag))
						{
							instance->flags[flag] = true;
						}
					}
					if (instance->savedObjects.Contains(currentLevel))
					{
						instance->savedObjects[currentLevel].data = objectWatcher->objects.data;
						instance->savedEnemies[currentLevel].data = objectWatcher->enemies.data;
					}
					else
					{
						instance->savedObjects.Add(currentLevel, objectWatcher->objects);
						instance->savedEnemies.Add(currentLevel, objectWatcher->enemies);
					}
					saveGameInstance->flags = instance->flags;
					saveGameInstance->savedObjects = instance->savedObjects;
					saveGameInstance->savedEnemies = instance->savedEnemies;
					//not sure about this.
					/*if (saveGameInstance->levelMusicPlaying.Contains(currentLevel))
					{
						saveGameInstance->levelMusicPlaying[currentLevel] = objectWatcher->instanceMusic;
					}
					else
					{
						saveGameInstance->levelMusicPlaying.Add(currentLevel, objectWatcher->instanceMusic);
					}*/
				}
				saveGameInstance->pendingLocks = instance->pendingLocks;
				if (container)
				{
					saveGameInstance->containerInventory = container->Inventory->inventory;
				}

				instance->numberOfSaves++;
				saveGameInstance->numberOfSaves = instance->numberOfSaves;

				FString slotName = "slot" + FString::FromInt(slot);
				FString slotMinimalName = "slotM" + FString::FromInt(slot);
				if (UGameplayStatics::SaveGameToSlot(saveGameInstance, slotName, 1))
				{
					if (UGameplayStatics::SaveGameToSlot(saveGameMinimalInstance, slotMinimalName, 1))
					{
						UE_LOG(LogTemp, Warning, TEXT("SAVED"));

					}
				}
			}
		}
	}
}

void AFixedCameraGameGameMode::LoadGame(int slot)
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		instance->LoadGame(slot);
	}
	// Retrieve and cast the USaveGame object to UMySaveGame.
/*	FString slotName = "slot" + FString::FromInt(slot);
	if (UFCSaveGame* loadedGame = Cast<UFCSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 1)))
	{
		auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (instance)
		{
			instance->currentLevel = loadedGame->currentLevel;
			auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			instance->startIndex = loadedGame->startIndex;
			instance->cameraIndex = loadedGame->cameraIndex;
			instance->playerInventory = loadedGame->playerInventory;
			instance->equippedIndex = loadedGame->equippedIndex;
			instance->playerHealth = loadedGame->playerHealth;
			instance->flags = loadedGame->flags;
			instance->savedObjects = loadedGame->savedObjects;
			instance->savedEnemies = loadedGame->savedEnemies;
			instance->containerInventory = loadedGame->containerInventory;
			instance->pendingLocks = loadedGame->pendingLocks;
		}

		UE_LOG(LogTemp, Warning, TEXT("LOADED current level %s"), *currentLevel);
		MoveToLevel(FName(*currentLevel));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no save in this slot %s"), *slotName);
	}*/
}

AActor* AFixedCameraGameGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

		for (int i = 0; i < FoundActors.Num(); i++)
		{
			auto start = Cast<AFC_PlayerStart>(FoundActors[i]);
			if (start->startIndex == instance->startIndex)
			{
				return start;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player start at index not found"));
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AFixedCameraGameGameMode::DisplayText(FString toDisplay, UFCLockComponent* lock, AFCInteractable* interactable, UFCSwitchComponent* Switch, bool advanceClear, AFCPlayerCamera* descriptionCamera, UFCInfoTextWidget* displayWidget)
{
	displayingText = true;
	if (displayWidget)
	{
		display = displayWidget;
	}
	else
	{
		display = CreateWidget<UFCInfoTextWidget>(GetWorld(), infoWidget);
		display->AddToViewport();
	}
	display->text = toDisplay;
	display->advanceClear = advanceClear;
	display->SetUpSegments();

	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//	pc->SetInputMode(FInputModeGameAndUI());
		//pc->SetInputMode(FInputModeUIOnly());

	auto playerCharacter = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	inMenu = false;
	if (playerCharacter)
	{
		inMenu = playerCharacter->inInventory;
	}
	if (!inMenu)
	{
		if (descriptionCamera)
		{
			StopEnemies(true, true);
			playerCharacter->GetMesh()->SetHiddenInGame(true, true);
			playerCharacter->SetActorTickEnabled(false);
			playerCharacter->inControl = false;
			fakePause = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("paused here"));
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
	else
	{
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
	}
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(pc);

	inputComponent = UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent;
	if (lock)
	{
		inputComponent->BindAction<InputLockDelegate>("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextToInventory, lock).bExecuteWhenPaused = true;
	}
	else if (interactable)
	{
		auto puzzle = Cast<AFCPuzzleInteractable>(interactable);
		if (Switch)
		{
			inputComponent->BindAction<InputSceneDelegate>("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextToScene, Switch, interactable).bExecuteWhenPaused = true;
		}
		else
		{
			inputComponent->BindAction<InputInteractDelegate>("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextToInteractable, interactable).bExecuteWhenPaused = true;
		}
	}
	else if (Switch)
	{
		inputComponent->BindAction<InputSceneDelegate>("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextToScene, Switch, interactable).bExecuteWhenPaused = true;
	}
	else if (descriptionCamera)
	{
		playerCamera = Cast<AFCPlayerCamera>(pc->GetViewTarget());
		FViewTargetTransitionParams transitionParams;
		pc->SetViewTarget(descriptionCamera, transitionParams);
		inputComponent->BindAction("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextFromCamera).bExecuteWhenPaused = true;

	}
	else
	{
		inputComponent->BindAction("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleText).bExecuteWhenPaused = true;
	}
	interactKey = inputComponent->GetNumActionBindings() - 1;
}

bool AFixedCameraGameGameMode::AdvanceText()
{
	bool finished = false;
	if (display->IsTextFinished())
	{
		finished = true;
	}
	else
	{
		if (display->IsSegmentFinished())
		{
			display->GetNextSegment();
		}
		else
		{
			display->delay = display->fastDelay;
		}
	}
	return finished;
}

void AFixedCameraGameGameMode::HandleText()
{
	if (AdvanceText() && display->advanceClear)
	{
		ClearText();
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

		if (!inMenu)
		{
			pc->SetInputMode(FInputModeGameOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			if (auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				player->ClearInventoryWidget();
			}
		}
		else
		{
			pc->SetInputMode(FInputModeGameAndUI());
			pc->SetShowMouseCursor(true);
		}
	}
}

void AFixedCameraGameGameMode::HandleTextToInteractable(AFCInteractable* interactable)
{
	if (AdvanceText())
	{
		ClearText();
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		auto puzzle = Cast<AFCPuzzleInteractable>(interactable);
		if (puzzle->active)
		{
			puzzle->StartPuzzle();
		}
		else
		{
			puzzle->ExitPuzzle();
		}
	}
}

void AFixedCameraGameGameMode::HandleTextToInventory(UFCLockComponent* lock)
{
	if (AdvanceText())
	{
		ClearText();
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

		auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		player->Toggle(2, lock, nullptr);
	}
}

void AFixedCameraGameGameMode::HandleTextToScene(UFCSwitchComponent* Switch, AFCInteractable* interactable)
{
	if (AdvanceText())
	{
		ClearText();
		if (auto puzzle = Cast<AFCPuzzleInteractable>(interactable))
		{
			puzzle->ExitPuzzle();
		}
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);
		pc->SetInputMode(FInputModeGameOnly());
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		Switch->PressSwitch();
	}
}

void AFixedCameraGameGameMode::HandleTextFromCamera()
{
	if (AdvanceText() && display->advanceClear)
	{
		ClearText();
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

		auto playerCharacter = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		playerCharacter->GetMesh()->SetHiddenInGame(false, true);
		playerCharacter->SetActorTickEnabled(true);
		playerCharacter->inControl = true;

		fakePause = false;

		FViewTargetTransitionParams transitionParams;
		pc->SetViewTarget(playerCamera, transitionParams);

		pc->SetInputMode(FInputModeGameOnly());
		ResumeEnemies();
		//UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

/*
void AFixedCameraGameGameMode::HandleTextToScene(AFCInteractable* interactable)
{
	if (AdvanceText())
	{
		ClearText();
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		auto puzzle = Cast<AFCPuzzleInteractable>(interactable);
		puzzle->StartPuzzle();

		Switch
	}
}
*/

void AFixedCameraGameGameMode::ClearText()
{
	display->RemoveFromParent();
	display = nullptr;
	inputComponent->RemoveActionBinding(interactKey);
	displayingText = false;
}

void AFixedCameraGameGameMode::LevelChangeSound(int32 index)
{
	if (exitSound[index])
	{
		auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		instance->entranceSoundIndex = index;
		UGameplayStatics::PlaySound2D(GetWorld(), exitSound[index]);
	}
}

FString AFixedCameraGameGameMode::GetDisplayName(FString name)
{
	return FName::NameToDisplayString(name, false);
}

FString AFixedCameraGameGameMode::GetTime()
{
	int seconds = fmod(gameTime, 60);
	int minutes = fmod(gameTime, 3600) / 60;
	int hours = gameTime / 3600;
	return FString::Printf(TEXT("%02d:%02d:%02d"), hours, minutes, seconds);
}
