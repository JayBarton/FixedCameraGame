// Fill out your copyright notice in the Description page of Project Settings.


#include "FCGameOverSequenceActor.h"
#include "FCPlayerCamera.h"
#include "FCGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Engine/SpotLight.h" 
#include "Components/SpotLightComponent.h" 

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
	camera->dynamicMaterial->SetVectorParameterValue("Color", FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	camera->SetMaterial(1.0f);
	levelFadeIn = true;
	currentCone = coneDefault;
	fadeOutLight->SpotLightComponent->SetOuterConeAngle(currentCone);


	//once I figure out how to get the weapon to appear that will be done here
	auto instance = Cast<UFCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	/*FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	currentWeapon = GetWorld()->SpawnActor<AFCWeapon>(weapon, SpawnInfo);
	currentWeapon->player = this;
	currentWeapon->Hide();*/

	if (instance->equippedIndex >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("right here"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nope"))
	}
}

// Called every frame
void AFCGameOverSequenceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentCone -= coneDescreaseSpeed * DeltaTime;
	fadeOutLight->SpotLightComponent->SetOuterConeAngle(currentCone);

	if (currentCone <= 0)
	{
		fadeOutLight->SetActorHiddenInGame(true);
	}

	currentRotation += rotationSpeed * DeltaTime;
	FVector newLocation = FVector(distanceFromCenter * cos(currentRotation), distanceFromCenter * sin(currentRotation), 360);
	camera->SetActorLocation(newLocation);
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(camera->GetActorLocation(), FVector(0));
	camera->SetActorRotation(newRotation);
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
		camera->UpdateMaterial(1.0f - transitionTimer, transitionTime);
		if (transitionTimer >= transitionTime)
		{
			transitionTimer = 0.0f;
			levelFadeIn = false;
			FTimerHandle DisplayTestTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DisplayTestTimerHandle, this, &AFCGameOverSequenceActor::FadeOut, textDisplayTime, false);
		}
	}

}

void AFCGameOverSequenceActor::FadeOut()
{
	camera->dynamicMaterial->SetVectorParameterValue("Color", FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	levelFadeOut = true;
}

