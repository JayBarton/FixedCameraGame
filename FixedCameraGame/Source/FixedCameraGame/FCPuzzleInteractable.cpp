// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPuzzleInteractable.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FixedCameraGameGameMode.h"

// Sets default values
AFCPuzzleInteractable::AFCPuzzleInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	prompt = "Use";
}

// Called when the game starts or when spawned
void AFCPuzzleInteractable::BeginPlay()
{
	Super::BeginPlay();
	if (linkedInteractable)
	{
		linkedLock = Cast<UFCLockComponent>(linkedInteractable->FindComponentByClass(UFCLockComponent::StaticClass()));
	}
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
}

void AFCPuzzleInteractable::OpenLock()
{
	UE_LOG(LogTemp, Warning, TEXT("T2"));
	//Not sure if this is the best way to do this, it should work.
	if (linkedLock)
	{
		linkedLock->Open(linkedLock->ID);
	}
	active = false;
	ExitPuzzle();
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *pc->GetViewTarget()->GetName());
	
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->DisplayText("Door unlocked");
	UE_LOG(LogTemp, Warning, TEXT("door unlocked"));
}

void AFCPuzzleInteractable::ExitPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Base"));
}



