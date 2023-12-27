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

void UFCSwitchComponent::PressSwitch(bool pauseAnimation, bool hidePlayer, bool playMusic, bool playOnce)
{
	switchState = !switchState;
	Switch.Broadcast(switchState);

	if (pressSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), pressSound);
	}

	if (playScene)
	{
		SetUpScene(pauseAnimation, hidePlayer, playMusic, playOnce);
	}
}

void UFCSwitchComponent::SetUpScene(bool pauseAnimation, bool hidePlayer, bool playMusic, bool playOnce)
{
	auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->inControl = false;
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(pc);
	UE_LOG(LogTemp, Warning, TEXT("HEY HEY"));
	FViewTargetTransitionParams transitionParams;
	pc->SetViewTarget(sceneCamera, transitionParams);

	if (hidePlayer)
	{
		auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		playerCharacter->GetMesh()->SetHiddenInGame(true, true);
	}

	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->StopEnemies(pauseAnimation);

	//I'm thinking I might be able to play a sort of chime, and I would probably want that to go here
	//For right now, I'm using this to turn on the music in East2 using the SpawnArea Blueprint
	//If I abandon the chime idea, I will remove this and call StartNewMusic directly in the SpawnArea blueprint
	if (playMusic)
	{
		gameMode->StartNewMusic(playOnce);
	}
	if (sceneLength > 0)
	{
		FTimerHandle SceneTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SceneTimerHandle, this, &UFCSwitchComponent::EndScene, sceneLength, false);
	}

	if (sceneSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), sceneSound);
	}
}

void UFCSwitchComponent::EndScene()
{
	auto player = Cast<AFCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->inControl = true;
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->EnableInput(pc);

	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	playerCharacter->GetMesh()->SetHiddenInGame(false, true);

	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->ResumeEnemies();

	FViewTargetTransitionParams transitionParams;
	pc->SetViewTarget(playerCamera, transitionParams);

	SwitchEnd.Broadcast();
}

