// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemySpawn.h"
#include "Components/ArrowComponent.h"
#include "Engine/Texture2D.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AFCEnemySpawn::AFCEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CapsuleComponent->ShapeColor = FColor(255, 138, 5, 255);
	CapsuleComponent->bDrawOnlyIfSelected = true;
	//CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->bShouldCollideWhenPlacing = true;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->Mobility = EComponentMobility::Static;
	CapsuleComponent->InitCapsuleSize(40.0f, 92.0f);
	CapsuleComponent->SetShouldUpdatePhysicsVolume(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = CapsuleComponent;
	bCollideWhenPlacing = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	ClickSprite = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	ClickSprite->bHiddenInGame = true;
	ClickSprite->SetupAttachment(CapsuleComponent);

	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->ArrowColor = FColor(150, 200, 255);
	ArrowComponent->ArrowSize = 1.0f;
	ArrowComponent->bTreatAsASprite = true;
	ArrowComponent->SetupAttachment(CapsuleComponent);
	ArrowComponent->bIsScreenSizeScaled = true;
}

// Called when the game starts or when spawned
void AFCEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	
}


