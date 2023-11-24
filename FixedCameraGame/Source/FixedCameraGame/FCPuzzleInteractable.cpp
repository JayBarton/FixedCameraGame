// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPuzzleInteractable.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FixedCameraGameGameMode.h"
#include "FCSwitchComponent.h"
#include "FCUnlockComponent.h"

// Sets default values
AFCPuzzleInteractable::AFCPuzzleInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	prompt = "Use";

	Switch = CreateDefaultSubobject<UFCSwitchComponent>(TEXT("Switch"));
	Unlocker = CreateDefaultSubobject<UFCUnlockComponent>(TEXT("Unlocker"));
}

// Called when the game starts or when spawned
void AFCPuzzleInteractable::BeginPlay()
{
	Super::BeginPlay();
}


void AFCPuzzleInteractable::Action_Implementation()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (active)
	{
		gameMode->DisplayText(puzzleDescription, nullptr, this);
	}
	else
	{
		gameMode->DisplayText(inactiveText);
	}
}

void AFCPuzzleInteractable::StartPuzzle()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->StopEnemies();
}

void AFCPuzzleInteractable::OpenLock()
{
	Unlocker->OpenLock();
	active = false;
	//ExitPuzzle();	
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(Switch->playScene)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("playing scene"));

		gameMode->DisplayText(puzzleCompleteText, nullptr, this, Switch);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no scene"));

		gameMode->DisplayText(puzzleCompleteText, nullptr, this);
		Switch->PressSwitch();
	}
}

void AFCPuzzleInteractable::ExitPuzzle()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->ResumeEnemies();
}



