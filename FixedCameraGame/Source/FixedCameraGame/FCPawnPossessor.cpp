// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPawnPossessor.h"
#include "FCPuzzlePawnBase.h"
#include "Camera/CameraActor.h" 
#include "Kismet/GameplayStatics.h"

#include "FCPlayer.h"


AFCPawnPossessor::AFCPawnPossessor()
{
}

void AFCPawnPossessor::StartPuzzle()
{
	Super::StartPuzzle();

	if (puzzlePawn)
	{
		if (newCamera)
		{
			auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			playerCharacter->GetMesh()->SetHiddenInGame(true, true);
			playerCharacter->SetActorTickEnabled(false);

			auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
			playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			pc->Possess(puzzlePawn);
			pc->SetViewTargetWithBlend(newCamera);

			puzzlePawn->StartPuzzle();
			puzzlePawn->parent = this;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No puzzle camera specified"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No puzzle pawn specified"))
	}
}

void AFCPawnPossessor::ExitPuzzle()
{
	Super::ExitPuzzle();

	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->Possess(playerPawn);
	pc->SetViewTargetWithBlend(playerCamera);

	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerCharacter->GetMesh()->SetHiddenInGame(false, true);
	playerCharacter->SetActorTickEnabled(true);
	//pc->SetViewTargetWithBlend(playerCamera, 1.0f, VTBlend_EaseInOut, 3.0f);
}
