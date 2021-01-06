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

#include "FCLockComponent.h"


AFixedCameraGameGameMode::AFixedCameraGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BPFCPlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
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
	currentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (instance)
	{
		CheckObjects(instance);

		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			pc->Inventory->inventory = instance->inventory;
		}

		FindStart(instance);
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
			instance->inventory = pc->Inventory->inventory;
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
			UGameplayStatics::OpenLevel(GetWorld(), levelName);
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

	return Super::ChoosePlayerStart_Implementation(Player);
}
