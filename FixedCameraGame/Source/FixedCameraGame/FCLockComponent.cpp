// Fill out your copyright notice in the Description page of Project Settings.


#include "FCLockComponent.h"

// Sets default values for this component's properties
UFCLockComponent::UFCLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFCLockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UFCLockComponent::Open(int32 keyID)
{
	if (keyID == ID)
	{
		DestroyComponent();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No matching key"));
	}
}


