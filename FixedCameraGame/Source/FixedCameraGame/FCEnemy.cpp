// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFCEnemy::AFCEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFCEnemy::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFCEnemy::OnNoiseHeard);
}

// Called when the game starts or when spawned
void AFCEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AFCEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFCEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		NoticePlayer();
	}
}

void AFCEnemy::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volumne)
{
	NoticePlayer();
}

void AFCEnemy::NoticePlayer()
{
	if (!hasNoticedPlayer)
	{
		hasNoticedPlayer = true;
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), player);
		UE_LOG(LogTemp, Warning, TEXT("Get up"));
	}
}
