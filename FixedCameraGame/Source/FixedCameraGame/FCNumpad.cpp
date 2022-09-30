// Fill out your copyright notice in the Description page of Project Settings.


#include "FCNumpad.h"
#include "Components/WidgetComponent.h" 
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"

#include "FCPlayer.h"

AFCNumpad::AFCNumpad()
{
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
}

void AFCNumpad::BeginPlay()
{
	Super::BeginPlay();

}


void AFCNumpad::StartPuzzle()
{
	Super::StartPuzzle();
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	//	pc->SetInputMode(FInputModeUIOnly());
		//pc->SetShowMouseCursor(false);

//	pc->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 3.0f);

	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerCharacter->GetMesh()->SetHiddenInGame(true, true);
	playerCharacter->SetActorTickEnabled(false);
	Cast<AFCPlayer>(playerCharacter)->inControl = false;

//	Widget->GetUserWidgetObject()->SetKeyboardFocus();
	SendToWidget();
}

void AFCNumpad::SendToWidget_Implementation()
{
}

void AFCNumpad::OpenLock()
{
	Super::OpenLock();

}

void AFCNumpad::ExitPuzzle()
{
	Super::ExitPuzzle();

	UE_LOG(LogTemp, Warning, TEXT("T1"));
	//Reset to player control
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	pc->SetInputMode(FInputModeGameOnly());
	pc->SetViewTargetWithBlend(playerCamera);
	//pc->SetViewTargetWithBlend(playerCamera, 1.0f, VTBlend_EaseInOut, 3.0f);
	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//playerCharacter->SetActorHiddenInGame(false);
	playerCharacter->GetMesh()->SetHiddenInGame(false, true);
	playerCharacter->SetActorTickEnabled(true);

	Cast<AFCPlayer>(playerCharacter)->inControl = true;

}


