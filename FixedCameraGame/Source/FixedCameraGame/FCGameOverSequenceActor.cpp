// Fill out your copyright notice in the Description page of Project Settings.


#include "FCGameOverSequenceActor.h"
#include "FCPlayerCamera.h"
#include "FCGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Engine/SpotLight.h" 
#include "Components/SpotLightComponent.h" 
#include "FCDeadPlayer.h"
#include "FCDeadWeapon.h"

// Sets default values
AFCGameOverSequenceActor::AFCGameOverSequenceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFCGameOverSequenceActor::BeginPlay()
{
	Super::BeginPlay();
	levelFadeIn = true;
	currentCone = coneDefault;
	fadeOutLight->SpotLightComponent->SetOuterConeAngle(currentCone);


	CheckWeapon();
}

//If the player died with a weapon equipped, spawn it on them
void AFCGameOverSequenceActor::CheckWeapon()
{
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (instance->equippedIndex >= 0)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto currentWeapon = GetWorld()->SpawnActor<AFCDeadWeapon>(weapon, SpawnInfo);

		currentWeapon->MeshComp->AttachToComponent(player->MeshComp, FAttachmentTransformRules::KeepRelativeTransform, player->WeaponAttachPoint);

	}
}

// Called every frame
void AFCGameOverSequenceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpotLightFade(DeltaTime);

	OrbitCamera(DeltaTime);

	CameraFade(DeltaTime);

}

void AFCGameOverSequenceActor::CameraFade(float DeltaTime)
{
	if (levelFadeOut)
	{
		transitionTimer += DeltaTime;
		//update camera
		camera->UpdateMaterial(transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			//return to main menu
			UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
		}
	}
	else if (levelFadeIn)
	{
		transitionTimer += DeltaTime;
		camera->UpdateMaterial(transitionTime - transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			transitionTimer = 0.0f;
			levelFadeIn = false;
			FTimerHandle DisplayTestTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DisplayTestTimerHandle, this, &AFCGameOverSequenceActor::FadeOut, textDisplayTime, false);
		}
	}
}

void AFCGameOverSequenceActor::OrbitCamera(float DeltaTime)
{
	currentRotation += rotationSpeed * DeltaTime;
	FVector newLocation = FVector(distanceFromCenter * cos(currentRotation), distanceFromCenter * sin(currentRotation), 360);
	camera->SetActorLocation(newLocation);
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(camera->GetActorLocation(), FVector(0));
	camera->SetActorRotation(newRotation);
}

//Handle fading out the main spotlight
void AFCGameOverSequenceActor::SpotLightFade(float DeltaTime)
{
	currentCone -= coneDescreaseSpeed * DeltaTime;
	fadeOutLight->SpotLightComponent->SetOuterConeAngle(currentCone);

	if (currentCone <= 0)
	{
		fadeOutLight->SetActorHiddenInGame(true);
	}
}

void AFCGameOverSequenceActor::FadeOut()
{
	camera->dynamicMaterial->SetVectorParameterValue("Color", FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	levelFadeOut = true;
}

