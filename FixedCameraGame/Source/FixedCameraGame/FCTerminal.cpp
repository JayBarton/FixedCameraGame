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



void AFCTerminal::BeginPlay()
{
	Super::BeginPlay();

	//I'm doing a short delay before checking the screen, as the gamemode handles clearing the lock and setting active, which may happen after begin play is called
	FTimerHandle CheckScreenTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CheckScreenTimerHandle, this, &AFCTerminal::CheckScreen, 0.2f, false);
}

void AFCTerminal::CheckScreen()
{
	auto lock = Cast<UFCLockComponent>(this->FindComponentByClass(UFCLockComponent::StaticClass()));
	if (!active || lock)
	{
		MeshComp->SetMaterial(1, blankMaterial);
		if (lock)
		{
			lock->Unlocked.AddDynamic(this, &AFCTerminal::Unlocked);
		}
		else if (!active)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damn you Chaos"));
		}
	}
	else
	{
		MeshComp->SetMaterial(1, onMaterial);
	}
}

void AFCTerminal::StartPuzzle()
{
	Super::StartPuzzle();
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	//	pc->SetInputMode(FInputModeUIOnly());
	//pc->SetShowMouseCursor(false);

	pc->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 3.0f);

	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerCharacter->GetMesh()->SetHiddenInGame(true, true);
	playerCharacter->SetActorTickEnabled(false);
	Cast<AFCPlayer>(playerCharacter)->inControl = false;

	Widget->GetUserWidgetObject()->SetKeyboardFocus();
	FTimerHandle CheckScreenTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CheckScreenTimerHandle, this, &AFCTerminal::SendToWidget, 1.2f, false);

//	SendToWidget();
	//timer to display keyboard?
}

void AFCTerminal::SendToWidget_Implementation()
{
}

void AFCTerminal::OpenLock()
{
	Super::OpenLock();
	MeshComp->SetMaterial(1, blankMaterial);

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
	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//playerCharacter->SetActorHiddenInGame(false);
	playerCharacter->GetMesh()->SetHiddenInGame(false, true);
	playerCharacter->SetActorTickEnabled(true);

	Cast<AFCPlayer>(playerCharacter)->inControl = true;

}

void AFCTerminal::Unlocked()
{
	MeshComp->SetMaterial(1, onMaterial);
}
