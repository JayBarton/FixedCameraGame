// Fill out your copyright notice in the Description page of Project Settings.


#include "FCTerminal.h"
#include "Components/WidgetComponent.h" 
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h" 
#include "FCLockComponent.h"
#include "Kismet/GameplayStatics.h"

AFCTerminal::AFCTerminal()
{
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

void AFCTerminal::BeginPlay()
{
	Super::BeginPlay();
	if (linkedInteractable)
	{
		linkedLock = Cast<UFCLockComponent>(linkedInteractable->FindComponentByClass(UFCLockComponent::StaticClass()));
	}
}


void AFCTerminal::Action_Implementation()
{
	if (active)
	{
		auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
		pc->SetInputMode(FInputModeUIOnly());
		Widget->GetUserWidgetObject()->SetKeyboardFocus();
		pc->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 3.0f);
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorHiddenInGame(true);
		SendToWidget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already used"));
	}
}


void AFCTerminal::SendToWidget_Implementation()
{
}

void AFCTerminal::OpenLock()
{
	UE_LOG(LogTemp, Warning, TEXT("Open lock once that's implemented"));
	//Not sure if this is the best way to do this, it should work.
	if (linkedLock)
	{
		linkedLock->Open(linkedLock->ID);
	}
	active = false;
	ExitTerminal();
}

void AFCTerminal::ExitTerminal()
{
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->SetInputMode(FInputModeGameOnly());
	pc->SetViewTargetWithBlend(playerCamera, 0.0f, VTBlend_Linear, 0.0f);
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorHiddenInGame(false);

	//Reset to player control
}

