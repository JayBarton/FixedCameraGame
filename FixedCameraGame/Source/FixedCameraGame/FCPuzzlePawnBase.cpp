// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPuzzlePawnBase.h"
#include "FCPawnPossessor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h" 

// Sets default values
AFCPuzzlePawnBase::AFCPuzzlePawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFCPuzzlePawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFCPuzzlePawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFCPuzzlePawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ExitPuzzle", IE_Pressed, this, &AFCPuzzlePawnBase::ExitPuzzle);

}

void AFCPuzzlePawnBase::CheckSolution()
{
}

void AFCPuzzlePawnBase::StartPuzzle()
{
}

//TODO this is not being properly called when puzzles with props are solved, meaning the props are not cleared out
void AFCPuzzlePawnBase::ExitPuzzle()
{
	parent->ExitPuzzle();
}

void AFCPuzzlePawnBase::SolvePuzzle()
{
	parent->OpenLock();
	ExitPuzzle();

}

