// Fill out your copyright notice in the Description page of Project Settings.


#include "FCTerminal.h"
#include "Components/WidgetComponent.h" 
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"

#include "FCPlayer.h"

AFCTerminal::AFCTerminal()
{
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

void AFCTerminal::StartPuzzle()
{
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	//	pc->SetInputMode(FInputModeUIOnly());
		//pc->SetShowMouseCursor(false);

	pc->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 3.0f);

	auto playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	playerPawn->SetActorHiddenInGame(true);
	playerPawn->SetActorTickEnabled(false);
	Cast<AFCPlayer>(playerPawn)->inControl = false;

	Widget->GetUserWidgetObject()->SetKeyboardFocus();
	SendToWidget();
}

void AFCTerminal::SendToWidget_Implementation()
{
}

void AFCTerminal::ExitPuzzle()
{
	Super::ExitPuzzle();

	UE_LOG(LogTemp, Warning, TEXT("T1"));
	//Reset to player control
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	pc->SetInputMode(FInputModeGameOnly());
	pc->SetViewTargetWithBlend(playerCamera);
	//pc->SetViewTargetWithBlend(playerCamera, 1.0f, VTBlend_EaseInOut, 3.0f);
	auto playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	playerPawn->SetActorHiddenInGame(false);
	playerPawn->SetActorTickEnabled(true);

	Cast<AFCPlayer>(playerPawn)->inControl = true;

}

