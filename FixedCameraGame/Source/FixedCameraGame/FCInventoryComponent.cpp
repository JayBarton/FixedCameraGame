// Fill out your copyright notice in the Description page of Project Settings.


#include "FCInventoryComponent.h"
#include "Blueprint/UserWidget.h" 

// Sets default values for this component's properties
UFCInventoryComponent::UFCInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	//Used for first time set up, inventory is copied from the instance afterwards
	if (inventory.Num() == 0)
	{
		inventory.Init(FItemStruct(), capacity);
	}
}

int32 UFCInventoryComponent::AddToInventory(FItemStruct newItem)
{	
	int32 addedIndex = FindFreeSlot();
	if (addedIndex >= 0)
	{
		inventory[addedIndex] = newItem;
	}
	return addedIndex;
}

int32 UFCInventoryComponent::FindFreeSlot()
{
	int32 freeIndex = -1;
	for (int i = 0; i < capacity; i++)
	{
		if (inventory[i].ID < 0)
		{
			freeIndex = i;
			break;
		}
	}
	return freeIndex;
}

void UFCInventoryComponent::RemoveFromInventory(int index)
{
	inventory[index] = FItemStruct();
}


