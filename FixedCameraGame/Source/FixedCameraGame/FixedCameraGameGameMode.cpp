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


AFixedCameraGameGameMode::AFixedCameraGameGameMode()
{
	PrimaryActorTick.bTickEvenWhenPaused = true;

}

void AFixedCameraGameGameMode::BeginPlay()
{

	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TArray<AActor*> FoundActors;

	if (objectWatcher == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Objectwatcher null"));
	}

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
		CheckObjects(instance);

		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			pc->Inventory->inventory = instance->playerInventory;
		}

		FindStart(instance);

		fadeOut = true;
		playerCamera = Cast<AFCPlayerCamera>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewTarget());
		//playerCamera->SetDynamicMaterial();
		playerCamera->SetMaterial(1.0f);
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AFixedCameraGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (fadeIn)
	{
		transitionTimer += DeltaTime;
		//update camera
		playerCamera->UpdateMaterial(transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			MoveToLevel(nextLevel);
		}
	}
	else if (fadeOut)
	{
		transitionTimer += DeltaTime;
		playerCamera->UpdateMaterial(1.0f - transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			transitionTimer = 0.0f;
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			fadeOut = false;
		}
	}

}

void AFixedCameraGameGameMode::CheckObjects(UFCGameInstance* instance)
{
	if (objectWatcher)
	{
		if (instance->savedObjects.Contains(currentLevel))
		{
			//if they are the same size we must have saved the data at some point
			if (objectWatcher->objects.data.Num() == instance->savedObjects[currentLevel].data.Num())
			{
				for (int i = 0; i < objectWatcher->objects.data.Num(); i++)
				{
					objectWatcher->objects.data[i].spawn = instance->savedObjects[currentLevel].data[i].spawn;
					objectWatcher->objects.data[i].locked = instance->savedObjects[currentLevel].data[i].locked;
					objectWatcher->objects.data[i].active = instance->savedObjects[currentLevel].data[i].active;
					objectWatcher->objects.data[i].puzzle = instance->savedObjects[currentLevel].data[i].puzzle;
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
					if (!object.puzzle)
					{
						if (auto interactable = Cast<AFCInteractable>(object.actor))
						{
							interactable->puzzle = false;
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Error"));
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("first time in %s"), *currentLevel);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no object watcher"));
	}
}

void AFixedCameraGameGameMode::FindStart(UFCGameInstance* instance)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		auto startCamera = Cast<AFCPlayerCamera>(FoundActors[i]);
		if (startCamera)
		{
			if (startCamera->startIndex == instance->startIndex)
			{
				APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (playerController)
				{
					FViewTargetTransitionParams transitionParams;
					playerController->SetViewTarget(startCamera, transitionParams);
					break;
				}
			}
		}
	}
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
			if (objectWatcher)
			{
				objectWatcher->UpdateObjects();
				if (instance->savedObjects.Contains(currentLevel))
				{
					instance->savedObjects[currentLevel].data = objectWatcher->objects.data;
				}
				else
				{
					instance->savedObjects.Add(currentLevel, objectWatcher->objects);
				}
			}

			if (container)
			{
				instance->containerInventory = container->Inventory->inventory;
			}
			fadeIn = true;
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

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AFixedCameraGameGameMode::DisplayText(FString toDisplay, UFCLockComponent* lock)
{
	display = CreateWidget<UFCInfoTextWidget>(GetWorld(), infoWidget);
	display->AddToViewport();
	display->text = toDisplay;
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//	pc->SetInputMode(FInputModeGameAndUI());
		//pc->SetInputMode(FInputModeUIOnly());
	alreadyPaused = true;
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Right here"));
		alreadyPaused = false;
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
		inputComponent->BindAction<InputDelegate>("Interact", IE_Pressed, this, &AFixedCameraGameGameMode::HandleTextToInventory, lock).bExecuteWhenPaused = true;
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
	if (display->index == display->text.Len())
	{
		finished = true;
	}
	else
	{
		display->delay = display->fastDelay;
	}
	return finished;
}

void AFixedCameraGameGameMode::HandleText()
{
	if (AdvanceText())
	{
		display->RemoveFromParent();
		display = nullptr;
		inputComponent->RemoveActionBinding(interactKey);
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);
		if (!alreadyPaused)
		{
			pc->SetInputMode(FInputModeGameOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
		else
		{
			pc->SetInputMode(FInputModeGameAndUI());
			pc->SetShowMouseCursor(true);
		}
	}
}

void AFixedCameraGameGameMode::HandleTextToInventory(UFCLockComponent* lock)
{
	if (AdvanceText())
	{
		display->RemoveFromParent();
		display = nullptr;
		inputComponent->RemoveActionBinding(interactKey);
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

		auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		player->Toggle(2, lock, nullptr);
	}
}
