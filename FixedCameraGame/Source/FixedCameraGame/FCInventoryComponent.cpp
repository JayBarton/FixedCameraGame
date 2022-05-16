// Fill out your copyright notice in the Description page of Project Settings.


#include "FCInventoryComponent.h"
#include "Blueprint/UserWidget.h" 
#include "FixedCameraGameGameMode.h"
#include "Kismet/GameplayStatics.h" 

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

void UFCInventoryComponent::CombineItem(int first, UFCInventoryComponent* otherInventory, int second)
{
	bool added = false;
	if (first != second)
	{
		FItemStruct& firstItem = inventory[first];
		FItemStruct& secondItem = otherInventory->inventory[second];
		if (firstItem.ID == secondItem.ID)
		{
			if (firstItem.amount < firstItem.maxCapacity && secondItem.amount < secondItem.maxCapacity)
			{
				int32 toAdd = secondItem.maxCapacity - secondItem.amount;
				if (firstItem.amount <= toAdd)
				{
					secondItem.amount += firstItem.amount;
					firstItem = FItemStruct();
				}
				else
				{
					secondItem.amount += toAdd;
					firstItem.amount -= toAdd;
				}
				added = true;
			}
		}
	}
	if (!added)
	{
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->DisplayText("Cannot combine these");
	}

	//return added;
}


