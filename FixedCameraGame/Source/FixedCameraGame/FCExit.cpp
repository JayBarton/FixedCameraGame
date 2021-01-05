// Fill out your copyright notice in the Description page of Project Settings.


#include "FCExit.h"
#include "Kismet/GameplayStatics.h"
#include "FCPlayer.h"
#include "FCInventoryComponent.h"
#include "FCGameInstance.h"

void AFCExit::Action_Implementation()
{

	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (pc)
		{
			instance->startIndex = index;
			instance->inventory = pc->Inventory->inventory;
			UGameplayStatics::OpenLevel(GetWorld(), levelName);
		}
	}
}
