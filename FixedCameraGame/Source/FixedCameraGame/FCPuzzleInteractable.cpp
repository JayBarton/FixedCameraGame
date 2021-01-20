// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPuzzleInteractable.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFCPuzzleInteractable::AFCPuzzleInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFCPuzzleInteractable::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Please"));
	if (linkedInteractable)
	{
		linkedLock = Cast<UFCLockComponent>(linkedInteractable->FindComponentByClass(UFCLockComponent::StaticClass()));
	}
}


void AFCPuzzleInteractable::Action_Implementation()
{
/*	if (active)
	{
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	//	pc->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 3.0f);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already used"));
	}*/
}

void AFCPuzzleInteractable::OpenLock()
{
	//Not sure if this is the best way to do this, it should work.
	if (linkedLock)
	{
		linkedLock->Open(linkedLock->ID);
	}
	active = false;
	ExitPuzzle();
}

void AFCPuzzleInteractable::ExitPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Base"));

}



