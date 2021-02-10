// Fill out your copyright notice in the Description page of Project Settings.


#include "FCDescription.h"
#include "FCInfoTextWidget.h"
#include "Kismet/GameplayStatics.h" 
#include "FixedCameraGameGameMode.h"

AFCDescription::AFCDescription()
{
	prompt = "Examine";
}

void AFCDescription::Action_Implementation()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->DisplayText(description);
}
