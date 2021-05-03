// Fill out your copyright notice in the Description page of Project Settings.


#include "FCUnlockComponent.h"
#include "FCInteractable.h"
#include "FCLockComponent.h"

UFCUnlockComponent::UFCUnlockComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFCUnlockComponent::BeginPlay()
{
	Super::BeginPlay();

	if (linkedInteractable)
	{
		linkedLock = Cast<UFCLockComponent>(linkedInteractable->FindComponentByClass(UFCLockComponent::StaticClass()));
	}
}

void UFCUnlockComponent::OpenLock()
{
	if (linkedLock)
	{
		linkedLock->Open(linkedLock->ID);
	}
}

