// Fill out your copyright notice in the Description page of Project Settings.


#include "FCMultiLockInteractable.h"
#include "FCLockComponent.h"
#include "FCSwitchComponent.h"
#include "FCMultiLockComponent.h"
#include "FCExit.h"
#include "FixedCameraGameGameMode.h"
#include "Kismet/GameplayStatics.h" 

AFCMultiLockInteractable::AFCMultiLockInteractable()
{
	lockComponent = CreateDefaultSubobject<UFCLockComponent>(TEXT("lockComponent"));
	switchComponent = CreateDefaultSubobject<UFCSwitchComponent>(TEXT("switchComponent"));
	switchComponent->SwitchEnd.AddDynamic(this, &AFCMultiLockInteractable::CheckDoor);
	switchComponent->Switch.AddDynamic(this, &AFCMultiLockInteractable::SuperUniqueFunction);
}

void AFCMultiLockInteractable::Action_Implementation()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->DisplayText("All good here");
}

void AFCMultiLockInteractable::SuperUniqueFunction(bool testing)
{
	if (auto multiReference = exit->FindComponentByClass<UFCMultiLockComponent>())
	{
		if (multiReference->CheckLocks())
		{
			unlocked = true;
		}
	}
}

void AFCMultiLockInteractable::CheckDoor()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (unlocked)
	{
		if (postLockText)
		{
			gameMode->DisplayText("We did it");
		}
	}
	else
	{
		gameMode->DisplayText("Something still missing");
	}
}


