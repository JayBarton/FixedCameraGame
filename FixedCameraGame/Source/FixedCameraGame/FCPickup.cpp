// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPickup.h"
#include "Kismet/GameplayStatics.h" 
#include "FCPlayer.h"
#include "FCInventoryComponent.h"
#include "Structs.h"

AFCPickup::AFCPickup()
{
	prompt = "Examine";
}

void AFCPickup::Action_Implementation()
{
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (pc->Inventory->AddToInventory(details) >= 0)
	{
		Destroy();
	}
}
