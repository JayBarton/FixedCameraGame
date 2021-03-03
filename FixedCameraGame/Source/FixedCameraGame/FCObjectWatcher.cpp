// Fill out your copyright notice in the Description page of Project Settings.


#include "FCObjectWatcher.h"
#include "FCLockComponent.h"
#include "FCInteractable.h"
#include "FCSwitchInteractable.h"
#include "FCSwitchComponent.h"
#include "FCEnemy.h"

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
	for (int i = 0; i < enemies.data.Num(); i++)
	{
		FEnemiesToWatch& enemy = enemies.data[i];
		if (enemy.spawn)
		{
			enemy.transform = enemy.spawnActor->GetActorTransform();
		}
	}
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
			//Since some pickups can be locked, need to make sure the pickup hasn't been destroyed(picked up) before
			//checking if it is locked. Otherwise, there is a possibility of a crash
			//To check this, we can just check if the object spawn was set to false above
			if (objects.data[i].spawn == false)
			{
				objects.data[i].locked = false;
			}
			else
			{
				auto lock = object.actor->FindComponentByClass(UFCLockComponent::StaticClass());
				if (!lock)
				{
					objects.data[i].locked = false;
				}
			}
		}
		if (object.active)
		{
			auto interactable = Cast<AFCInteractable>(object.actor);
			if (interactable && !interactable->active)
			{
				objects.data[i].active = interactable->active;
			}
		}
		if (object.switched)
		{
			auto switchComponent = Cast<UFCSwitchComponent>(object.actor->FindComponentByClass(UFCSwitchComponent::StaticClass()));
			if (switchComponent)
			{
				objects.data[i].switched = switchComponent->switchState;
			}
		}
	}

	for (int i = 0; i < enemies.data.Num(); i++)
	{
		FEnemiesToWatch& enemy = enemies.data[i];
		if (enemy.spawn)
		{
			if (enemy.enemy->dead)
			{
				enemy.alive = false;
				enemy.transform = enemy.enemy->GetTransform();
			}
			else
			{
				enemy.alive = true;
				enemy.transform = enemy.spawnActor->GetTransform();
			}
			if (enemy.spawnIn)
			{
				enemy.spawnIn = enemy.enemy->spawnIn;
			}
		}
	}
}


