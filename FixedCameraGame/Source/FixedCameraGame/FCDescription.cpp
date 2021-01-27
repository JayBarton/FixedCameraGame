// Fill out your copyright notice in the Description page of Project Settings.


#include "FCDescription.h"
#include "FCInfoTextWidget.h"
#include "Kismet/GameplayStatics.h" 
#include "FixedCameraGameGameMode.h"

AFCDescription::AFCDescription()
{
	prompt = "Examine";
}

/*void AFCDescription::Action_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("This is a description"));

}*/

void AFCDescription::Action_Implementation()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->DisplayText(description);
	
	/*auto display = CreateWidget<UFCInfoTextWidget>(GetWorld(), infoWidget);
	display->AddToViewport();
	display->text = description;
	display->SetKeyboardFocus();
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->SetInputMode(FInputModeUIOnly());
	UGameplayStatics::SetGamePaused(GetWorld(), true);*/

}
