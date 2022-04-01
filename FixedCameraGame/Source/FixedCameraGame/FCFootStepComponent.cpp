// Fill out your copyright notice in the Description page of Project Settings.


#include "FCFootStepComponent.h"
#include "Components/AudioComponent.h" 
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values for this component's properties
UFCFootStepComponent::UFCFootStepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UFCFootStepComponent::FootstepSound()
{
	auto actor = GetOwner();
	FHitResult OutHit;

	FVector Start = actor->GetActorLocation();

	FVector End = Start - FVector(0.0f, 0.0f, 180.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(actor);
	QueryParams.bReturnPhysicalMaterial = true;

	EPhysicalSurface SurfaceType = SurfaceType_Default;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams))
	{
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());
	}
	auto audioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), footstep, Start);
	int surfaceIndex = 0;
	if (surfaceStep.Contains(SurfaceType))
	{
		surfaceIndex = surfaceStep[SurfaceType];
	}
	audioComponent->SetIntParameter("Surface", surfaceIndex);
	actor->MakeNoise();
}

