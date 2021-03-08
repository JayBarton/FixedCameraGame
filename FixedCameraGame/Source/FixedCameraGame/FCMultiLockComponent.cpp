// Fill out your copyright notice in the Description page of Project Settings.


#include "FCMultiLockComponent.h"
#include "FCLockComponent.h"
#include "FCInteractable.h"

// Sets default values for this component's properties
UFCMultiLockComponent::UFCMultiLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UFCMultiLockComponent::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < Locks.Num(); i++)
	{
		if (Locks[i].LockedActor)
		{
			Locks[i].Lock = Cast<UFCLockComponent>(Locks[i].LockedActor->FindComponentByClass(UFCLockComponent::StaticClass()));
		}
	}
}


// Called every frame
void UFCMultiLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFCMultiLockComponent::CheckLocks()
{
	bool unLocked = false;
	for (int i = 0; i < Locks.Num(); i++)
	{
		if (!IsValid(Locks[i].Lock))
		{
			Locks.RemoveAt(i);
			i--;
		}
	}
	if (Locks.Num() == 0)
	{
		DestroyComponent();
		unLocked = true;
	}
	return unLocked;
}

