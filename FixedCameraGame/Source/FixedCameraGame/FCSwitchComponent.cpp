// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSwitchComponent.h"

// Sets default values for this component's properties
UFCSwitchComponent::UFCSwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFCSwitchComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFCSwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFCSwitchComponent::PressSwitch()
{
	switchState = !switchState;
	Switch.Broadcast(switchState);
}

