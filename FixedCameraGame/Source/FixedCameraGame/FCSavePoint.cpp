// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSavePoint.h"
#include "Kismet/GameplayStatics.h" 
#include "FCPlayer.h"
#include "FCInventoryComponent.h"
#include "FixedCameraGameGameMode.h"

AFCSavePoint::AFCSavePoint()
{
	prompt = "Save";
}

void AFCSavePoint::Action_Implementation()
{
	bool haveToken = false;
	int token = -1;
	auto pc = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto Inventory = pc->Inventory;
	for (int i = 0; i < Inventory->inventory.Num(); i++)
	{
		FItemStruct& slot = Inventory->inventory[i];
		if (slot.ID == saveID)
		{
			haveToken = true;
			token = i;
			break;
		}
	}
	if (haveToken)
	{
		pc->CreateSaveWidget(token, playerStartIndex);
	}
	else
	{
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		FString display = "You could save your game if you had a save token...";
		gameMode->DisplayText(display);
	}

}
