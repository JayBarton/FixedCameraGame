// Fill out your copyright notice in the Description page of Project Settings.


#include "FCCameraSwitchVolume.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "FCPlayerCamera.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFCCameraSwitchVolume::AFCCameraSwitchVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollison = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = boxCollison;

	boxCollison->SetBoxExtent(FVector(1.5f, 32.0f, 32.0f));

	textRenderNewCamera = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text New Camera"));
	textRenderNewCamera->SetupAttachment(RootComponent);
	textRenderNewCamera->SetText("New Camera Side");
	textRenderNewCamera->SetHiddenInGame(true);
	textRenderNewCamera->SetRelativeLocation(FVector(15.0f, 0.0f, 0.0f));
	textRenderNewCamera->SetHorizontalAlignment(EHTA_Center);
	textRenderNewCamera->SetVerticalAlignment(EVRTA_TextCenter);

	textRenderPreviousCamera = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Previous Camera"));
	textRenderPreviousCamera->SetupAttachment(RootComponent);
	textRenderPreviousCamera->SetText("Previous Camera Sidewwwe");
	textRenderPreviousCamera->SetHiddenInGame(true);
	textRenderPreviousCamera->SetRelativeLocation(FVector(-15.0f, 0.0f, 0.0f));
	textRenderPreviousCamera->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	textRenderPreviousCamera->SetHorizontalAlignment(EHTA_Center);
	textRenderPreviousCamera->SetVerticalAlignment(EVRTA_TextCenter);
}

// Called when the game starts or when spawned
void AFCCameraSwitchVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorEndOverlap.AddDynamic(this, &AFCCameraSwitchVolume::OnEndOverlap);
}

// Called every frame
void AFCCameraSwitchVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFCCameraSwitchVolume::OnConstruction(const FTransform& Transform)
{
	boxCollison->SetBoxExtent(FVector(1.5f, width, height));
}

void AFCCameraSwitchVolume::OnEndOverlap(AActor* thisActor, AActor* otherActor)
{

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//Switch camera based on player's relative location
	if (playerController && playerCharacter && otherActor == playerCharacter)
	{
		FViewTargetTransitionParams transitionParams;
		FVector playerRelativeLocation = GetTransform().InverseTransformPosition(playerCharacter->GetActorLocation());
		
		//If player exits new camera side
		if (newCamera && playerRelativeLocation.X > 0)
		{
			playerController->SetViewTarget(newCamera, transitionParams);
		}
		//If player exits previous camera side
		if (previousCamera && playerRelativeLocation.X < 0)
		{
			playerController->SetViewTarget(previousCamera, transitionParams);
		}
	}

}

