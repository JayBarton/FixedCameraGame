// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPuzzleIneractable.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFCPuzzleIneractable::AFCPuzzleIneractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFCPuzzleIneractable::BeginPlay()
{
	Super::BeginPlay();
	if (linkedInteractable)
	{
		linkedLock = Cast<UFCLockComponent>(linkedInteractable->FindComponentByClass(UFCLockComponent::StaticClass()));
	}
}


void AFCPuzzleIneractable::Action_Implementation()
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

void AFCPuzzleIneractable::OpenLock()
{
	//Not sure if this is the best way to do this, it should work.
	if (linkedLock)
	{
		linkedLock->Open(linkedLock->ID);
	}
	active = false;
	ExitPuzzle();
}

void AFCPuzzleIneractable::ExitPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Base"));

}



