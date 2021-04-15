// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSwitchComponent.h"
#include "Components/TimelineComponent.h" 
#include "Kismet/GameplayStatics.h" 
#include "FCPlayer.h"
#include "FCPlayerCamera.h"
#include "FixedCameraGameGameMode.h"

// Sets default values for this component's properties
UFCSwitchComponent::UFCSwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UFCSwitchComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UFCSwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFCSwitchComponent::PressSwitch(bool pauseAnimation)
{
	switchState = !switchState;
	Switch.Broadcast(switchState);
	if (playScene)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pass"));
		SetUpScene(pauseAnimation);
	}
}

void UFCSwitchComponent::SetUpScene(bool pauseAnimation)
{
	UE_LOG(LogTemp, Warning, TEXT("Setup"));
	auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->inControl = false;
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(pc);
	FViewTargetTransitionParams transitionParams;
	pc->SetViewTarget(sceneCamera, transitionParams);

	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->StopEnemies(pauseAnimation);

	FTimerHandle SceneTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SceneTimerHandle, this, &UFCSwitchComponent::EndScene, sceneLength, false);
}

void UFCSwitchComponent::EndScene()
{
	UE_LOG(LogTemp, Warning, TEXT("finish"));

	auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->inControl = true;
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->ResumeEnemies();

	FViewTargetTransitionParams transitionParams;
	pc->SetViewTarget(playerCamera, transitionParams);

	SwitchEnd.Broadcast();
}

