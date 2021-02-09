// Fill out your copyright notice in the Description page of Project Settings.


#include "FCObjectWatcher.h"
#include "FCLockComponent.h"
#include "FCInteractable.h"

// Sets default values
AFCObjectWatcher::AFCObjectWatcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFCObjectWatcher::BeginPlay()
{
	Super::BeginPlay();

}


void AFCObjectWatcher::UpdateObjects()
{
	for (int i = 0; i < objects.data.Num(); i++)
	{
		auto object = objects.data[i];
		if (object.spawn)
		{
			if (!IsValid(object.actor))
			{
				objects.data[i].spawn = false;
			}
		}
		if (object.locked)
		{
			auto lock = object.actor->FindComponentByClass(UFCLockComponent::StaticClass());
			if (!lock)
			{
				objects.data[i].locked = false;
			}
		}
		if (object.active)
		{
			auto interactable = Cast<AFCInteractable>(object.actor);
			if (interactable && !interactable->active)
			{
				objects.data[i].active = false;
			}
		}
	}
}


