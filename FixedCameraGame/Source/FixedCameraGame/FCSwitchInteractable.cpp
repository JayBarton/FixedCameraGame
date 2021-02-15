// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSwitchInteractable.h"
#include "FCPlayer.h"
#include "Kismet/GameplayStatics.h" 

AFCSwitchInteractable::AFCSwitchInteractable()
{
	prompt = "turn on";
}

void AFCSwitchInteractable::Action_Implementation()
{
	//Will create a widget here...
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	pc->CreateSwitchWidget(this);

}

void AFCSwitchInteractable::PressSwitch()
{
	switchState = !switchState;
	Switch.Broadcast(switchState);
}
