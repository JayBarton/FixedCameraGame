// Fill out your copyright notice in the Description page of Project Settings.


#include "FCInventoryComponent.h"
#include "Blueprint/UserWidget.h" 

// Sets default values for this component's properties
UFCInventoryComponent::UFCInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//inventoryWindow = CreateDefaultSubobject<UUserWidget>(TEXT("InventoryWindow"));
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
	int32 addedIndex = -1;
	for (int i = 0; i < capacity; i++)
	{
		if (inventory[i].ID < 0)
		{
			inventory[i] = newItem;
			addedIndex = i;
			break;
		}
	}

	return addedIndex;
}

void UFCInventoryComponent::RemoveFromInventory(int index)
{
	inventory[index] = FItemStruct();
}


