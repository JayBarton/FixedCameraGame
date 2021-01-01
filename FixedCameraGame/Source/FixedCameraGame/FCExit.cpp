// Fill out your copyright notice in the Description page of Project Settings.


#include "FCExit.h"
#include "Kismet/GameplayStatics.h" 
#include "FCGameInstance.h"

void AFCExit::Action_Implementation()
{

	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (instance)
	{
		instance->startIndex = index;
		UGameplayStatics::OpenLevel(GetWorld(), levelName);
	}
}
