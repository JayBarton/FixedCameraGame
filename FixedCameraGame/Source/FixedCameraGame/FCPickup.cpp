// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPickup.h"
#include "Kismet/GameplayStatics.h" 
#include "Engine/DataTable.h" 
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
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (pc->Inventory->FindFreeSlot() >= 0)
	{
		//display "do you want to take [item]?" text
		//create widget
		pc->CreatePickUpWidget(this);
	}
	else
	{
		pc->DisplayInventoryWidget();
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		FString display = "Can't take " + details.name.ToString() + ". Inventory full";
		gameMode->DisplayText(display);
	}
}

void AFCPickup::BeginPlay()
{
	Super::BeginPlay();

	if (data)
	{
		auto toLoad = data->FindRow<FItemStruct>(FName(*FString::FromInt(details.ID)), "");
		if (toLoad)
		{
			details = *toLoad;
		}
	}
}

int32 AFCPickup::TakeItem()
{
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int32 pickedUpIndex = pc->Inventory->AddToInventory(details);
	if (pickedUpIndex >= 0)
	{

		Destroy();
	}
	//Don't think this is in use right now...
	else
	{
		//reset player input
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->DisplayText("Inventory full");
	}
	return pickedUpIndex;
}
