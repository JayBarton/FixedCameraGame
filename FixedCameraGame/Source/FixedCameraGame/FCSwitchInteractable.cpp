// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSwitchInteractable.h"
#include "FCPlayer.h"
#include "FixedCameraGameGameMode.h"
#include "FCSwitchComponent.h"
#include "Kismet/GameplayStatics.h" 

AFCSwitchInteractable::AFCSwitchInteractable()
{
	prompt = "turn on";
	Switch = CreateDefaultSubobject<UFCSwitchComponent>(TEXT("Switch"));

}

void AFCSwitchInteractable::Action_Implementation()
{
	if (active)
	{
		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		pc->CreateSwitchWidget(this);
	}
	else
	{
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->DisplayText(inactiveText);
	}
}

void AFCSwitchInteractable::PressSwitch()
{
	//switchState = !switchState;
	//Switch.Broadcast(switchState);
	Switch->PressSwitch();
	if (!isToggle)
	{
		//Need to make sure any affected objects are also saved by the object watcher
		active = false;
	}
}
