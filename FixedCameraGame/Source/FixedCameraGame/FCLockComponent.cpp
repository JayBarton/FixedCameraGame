// Fill out your copyright notice in the Description page of Project Settings.


#include "FCLockComponent.h"
#include "FCInteractable.h"
#include "FCSwitchComponent.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values for this component's properties
UFCLockComponent::UFCLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(TEXT("/Game/Audio/Doors/handlelock2.handlelock2"));
	if (SoundAsset.Succeeded())
	{
		lockedSound = SoundAsset.Object;
	}
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
		Unlocked.Broadcast();
		if (auto owner = Cast<AFCInteractable>(GetOwner()))
		{
			if (owner->unlockSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), owner->unlockSound);
			}

		}
		DestroyComponent();
	}
	//pretty sure this isn't in use...
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No matching key"));
	}
}


