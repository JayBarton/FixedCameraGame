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

void AFCPuzzlePawnBase::StartPuzzle()
{
}

void AFCPuzzlePawnBase::ExitPuzzle()
{
	parent->ExitPuzzle();
	/*auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->Possess(playerPawn);
	pc->SetViewTargetWithBlend(playerCamera);*/
}

void AFCPuzzlePawnBase::SolvePuzzle()
{
	parent->OpenLock();
}

