// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSwitchObjectBase.h"
#include "FCSwitchInteractable.h"

// Sets default values
AFCSwitchObjectBase::AFCSwitchObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFCSwitchObjectBase::BeginPlay()
{
	Super::BeginPlay();
	if (attachedSwitch)
	{
	//	attachedSwitch->SwitchOn.AddDynamic(this, &AFCSwitchObjectBase::SetToOnState);
	//	attachedSwitch->Switch.AddDynamic(this, &AFCSwitchObjectBase::SwitchAction);
	}
}

// Called every frame
void AFCSwitchObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFCSwitchObjectBase::SwitchAction(bool isOn)
{
	if (isOn)
	{
		UE_LOG(LogTemp, Warning, TEXT("On"));
	}
}

void AFCSwitchObjectBase::SetToOnState()
{
	UE_LOG(LogTemp, Warning, TEXT("Testing this"));

}


