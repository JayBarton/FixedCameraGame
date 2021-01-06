// Fill out your copyright notice in the Description page of Project Settings.


#include "FCExit.h"
#include "Kismet/GameplayStatics.h"
#include "FixedCameraGameGameMode.h"

void AFCExit::Action_Implementation()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>( UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->ChangeLevel(index, levelName);
}
