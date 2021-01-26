// Fill out your copyright notice in the Description page of Project Settings.


#include "FCContainer.h"
#include "FCInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FCPlayer.h"

AFCContainer::AFCContainer()
{
	Inventory = CreateDefaultSubobject<UFCInventoryComponent>(TEXT("Inventory"));
	Inventory->capacity = 12;
	prompt = "Open";
}

/*void AFCContainer::Action_Implementation()
{
	auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


}*/