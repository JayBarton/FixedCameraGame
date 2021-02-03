// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPickup.h"
#include "Kismet/GameplayStatics.h" 
#include "FCPlayer.h"
#include "FCInventoryComponent.h"
#include "Structs.h"

#include "FixedCameraGameGameMode.h"

AFCPickup::AFCPickup()
{
	prompt = "Examine";
}

void AFCPickup::Action_Implementation()
{
	//TakeItem();
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	pc->CreatePickUpWidget(this);
	UE_LOG(LogTemp, Warning, TEXT("HAET"));
	//display "do you want to take [item]?" text
	//create widget
}

bool AFCPickup::TakeItem()
{
	bool pickedUp = false;
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (pc->Inventory->AddToInventory(details) >= 0)
	{
		//display text
		pickedUp = true;
		//remove widget
		//reset game input
		//reset player input
		Destroy();
	}
	else
	{
		//reset player input
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->DisplayText("Inventory full");
	}
	return pickedUp;
}
