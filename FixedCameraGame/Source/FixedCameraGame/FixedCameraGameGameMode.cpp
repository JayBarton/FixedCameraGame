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
#include "FCPuzzleInteractable.h"
#include "FCSwitchInteractable.h"
#include "FCSwitchComponent.h"
#include "FCExit.h"
#include "FCEnemy.h"


AFixedCameraGameGameMode::AFixedCameraGameGameMode()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFCContainer::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		container = Cast<AFCContainer>(FoundActors[0]);
		if (instance->containerInventory.Num() == container->Inventory->inventory.Num())
		{
			container->Inventory->inventory = instance->containerInventory;
		}
	}

	currentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (instance)
	{
		CheckInstance(instance);

		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			pc->Inventory->inventory = instance->playerInventory;
			pc->equipped = instance->equippedIndex;
		}

		FindStart(instance);

		levelFadeIn = true;
		playerCamera->SetMaterial(1.0f);
	//	UGameplayStatics::SetGamePaused(GetWorld(), true);
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
			//UGameplayStatics::SetGamePaused(GetWorld(), false);
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
}

void AFixedCameraGameGameMode::CheckInstance(UFCGameInstance* instance)
{
	if (objectWatcher)
	{
		if (instance->savedObjects.Contains(currentLevel))
		{
			CheckObjects(instance);
			CheckEnemies(instance);
		}
		else
		{
			SpawnEnemies(instance);
			UE_LOG(LogTemp, Warning, TEXT("first time in %s"), *currentLevel);
		}
		HandlePendingLocks(instance);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no object watcher"));
	}
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
						lock->DestroyComponent();
					}
				}
			}
			if (!object.active)
			{
				if (auto interactable = Cast<AFCInteractable>(object.actor))
				{
					interactable->active = false;
				}
			}
			if (object.switched)
			{
				if (auto switchComponent = Cast<UFCSwitchComponent>(object.actor->FindComponentByClass(UFCSwitchComponent::StaticClass())))
				{
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
			objectWatcher->enemies.data[i].alive = instance->savedEnemies[currentLevel].data[i].alive;
			objectWatcher->enemies.data[i].spawnIn = instance->savedEnemies[currentLevel].data[i].spawnIn;
			objectWatcher->enemies.data[i].transform = instance->savedEnemies[currentLevel].data[i].transform;
		}

		UE_LOG(LogTemp, Warning, TEXT("Should be here?"));

		SpawnEnemies(instance);
	}
}

void AFixedCameraGameGameMode::SpawnEnemies(UFCGameInstance* instance)
{
	UE_LOG(LogTemp, Warning, TEXT("spawn"));

	for (int i = 0; i < objectWatcher->enemies.data.Num(); i++)
	{
		FEnemiesToWatch& enemy = objectWatcher->enemies.data[i];
		if (enemy.spawnFlag == "")
		{
			enemy.spawn = true;
		}
		else
		{
			if (instance->flags.Contains(enemy.spawnFlag))
			{
				enemy.spawn = instance->flags[enemy.spawnFlag];
			}
			else
			{
				//Don't know that this else is needed, as it's not like I'll be creating flags on the fly
				//keeping it around for now until I finalize this flag idea
				enemy.spawn = false;
			}
		}
		if (enemy.spawn)
		{

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			enemy.enemy = GetWorld()->SpawnActor<AFCEnemy>(enemy.enemyType, enemy.transform, SpawnParams);
			if (!enemy.alive)
			{
				enemy.enemy->SetActorTransform(enemy.transform);
				//enemy.enemy->dead = true;
				//enemy.enemy->Kill();
				enemy.enemy->StartDead();
			}
			else if (enemy.spawnIn)
			{
				enemy.enemy->spawnIn = true;
				UE_LOG(LogTemp, Warning, TEXT("Least that we can do is stand for what we think is right"));
				//waiting to spawn
			}
			//enemy.enemy->Destroy();
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
			if (playerCamera->startIndex == instance->startIndex)
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

void AFixedCameraGameGameMode::ChangeLevel(int index, FName levelName)
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			instance->startIndex = index;
			instance->playerInventory = pc->Inventory->inventory;
			instance->equippedIndex = pc->equipped;
			if (objectWatcher)
			{
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
			}

			if (container)
			{
				instance->containerInventory = container->Inventory->inventory;
			}
			levelFadeOut = true;
			nextLevel = levelName;
			playerCamera = Cast<AFCPlayerCamera>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewTarget());
			UGameplayStatics::SetGamePaused(GetWorld(), true);

		}
	}
}

void AFixedCameraGameGameMode::MoveToLevel(FName levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), levelName);
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

void AFixedCameraGameGameMode::DisplayText(FString toDisplay, UFCLockComponent* lock, AFCInteractable* interactable, bool advanceClear)
{
	display = CreateWidget<UFCInfoTextWidget>(GetWorld(), infoWidget);
	display->AddToViewport();
	display->text = toDisplay;
	display->advanceClear = advanceClear;
	display->SetUpSegments();

	UE_LOG(LogTemp, Warning, TEXT("segments %i"), display->numberOfSegments);


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
		UE_LOG(LogTemp, Warning, TEXT("paused here"));
		UGameplayStatics::SetGamePaused(GetWorld(), true);
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
		inputComponent->BindAction<InputInteractDelegate>("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextToInteractable, interactable).bExecuteWhenPaused = true;
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
			UE_LOG(LogTemp, Warning, TEXT("where am i"));

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
		puzzle->StartPuzzle();
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

void AFixedCameraGameGameMode::ClearText()
{
	display->RemoveFromParent();
	display = nullptr;
	inputComponent->RemoveActionBinding(interactKey);
}
