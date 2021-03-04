// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"

// Sets default values
AFCEnemy::AFCEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFCEnemy::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFCEnemy::OnNoiseHeard);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetWorldScale3D(FVector(4.0f, 4.0f, 2.75f));
	BoxComp->SetCollisionProfileName(TEXT("EnemyBox"));
	/*BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);*/
	BoxComp->SetupAttachment(RootComponent);
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
	if (spawnIn)
	{
		spawnIn = false;
		spawning = true;
	}
	else
	{
		if (!hasNoticedPlayer && !isAttacking && !staggered && !spawning)
		{
			if (dead)
			{
				dead = false;
			}
			else
			{
				hasNoticedPlayer = true;
				/*if (AAIController* AI = Cast<AAIController>(GetController()))
				{
					AI->MoveToActor(player);
				}*/
				UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), player);
			}
		}
	}
}

void AFCEnemy::Attack()
{

}

void AFCEnemy::FinishAttack()
{
	isAttacking = false;
}

void AFCEnemy::RecoverFromStagger()
{
	if (!dead)
	{
		staggered = false;
		NoticePlayer();
		hitDirection = HitDirection::NONE;
	}
}


void AFCEnemy::TakeDamage(int32 damageAmount, FHitResult Hit)
{
	DetermineImpactDirection(Hit);
	hp -= damageAmount;
	if (hp <= 0)
	{
		if (AController* AI = GetController())
		{
			AI->StopMovement();
		}
		Kill();
	}
	else
	{
		if (!staggered)
		{
			staggerHits++;
			if (staggerHits >= hitsToStagger)
			{
				Stagger();
			}
			GetWorld()->GetTimerManager().SetTimer(StaggerTimerHandle, this, &AFCEnemy::ResetStaggerHits, 0.6f, false);
		}
	}
}

void AFCEnemy::Stagger()
{
	if (AController* AI = GetController())
	{
		AI->StopMovement();
	}
	staggered = true;
	isAttacking = false;
	hasNoticedPlayer = false;
	UE_LOG(LogTemp, Warning, TEXT("Rstop here r"));

	FTimerHandle StaggerRecoverTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StaggerRecoverTimerHandle, this, &AFCEnemy::RecoverFromStagger, 0.97f, false);

}

void AFCEnemy::ResetStaggerHits()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Stagger"));

	staggerHits = 0;
}

void AFCEnemy::DetermineImpactDirection(FHitResult& Hit)
{
	if (ImpactFX)
	{
		int dot = FMath::RoundToInt(FVector::DotProduct(Hit.ImpactNormal, GetActorForwardVector()));

		if (dot == 1)
		{
			hitDirection = HitDirection::FRONT;
			int32 index = FMath::RandRange(0, FrontImpactPoints.Num() - 1);
			UGameplayStatics::SpawnEmitterAttached(ImpactFX, GetMesh(), FrontImpactPoints[index], FVector(0), FRotator(0));
			//UE_LOG(LogTemp, Warning, TEXT("front"));
		}
		else if (dot == -1)
		{
			hitDirection = HitDirection::BACK;
			int32 index = FMath::RandRange(0, BackImpactPoints.Num() - 1);
			UGameplayStatics::SpawnEmitterAttached(ImpactFX, GetMesh(), BackImpactPoints[index], FVector(0), FRotator(0));
			//UE_LOG(LogTemp, Warning, TEXT("back"));
		}
		else
		{
			dot = FMath::RoundToInt(FVector::DotProduct(Hit.ImpactNormal, GetActorRightVector()));
			if (dot == 1)
			{
				hitDirection = HitDirection::RIGHT;
				int32 index = FMath::RandRange(0, RightImpactPoints.Num() - 1);
				UGameplayStatics::SpawnEmitterAttached(ImpactFX, GetMesh(), RightImpactPoints[index], FVector(0), FRotator(0));
				//UE_LOG(LogTemp, Warning, TEXT("right"));
			}
			else if (dot == -1)
			{
				hitDirection = HitDirection::LEFT;
				int32 index = FMath::RandRange(0, LeftImpactPoints.Num() - 1);
				UGameplayStatics::SpawnEmitterAttached(ImpactFX, GetMesh(), LeftImpactPoints[index], FVector(0), FRotator(0));
				//	UE_LOG(LogTemp, Warning, TEXT("left"));
			}
		}
	}
}

void AFCEnemy::Kill()
{
	PawnSensingComp->SetSensingUpdatesEnabled(false);
	SetActorTickEnabled(false);
	dead = true;
	SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("stop moving, stop it!"));

}

void AFCEnemy::StartDead()
{
	int32 index = FMath::RandRange(0, 1);
	dead = true;

	if (index == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("revive"));
	}
	else
	{
		PawnSensingComp->SetSensingUpdatesEnabled(false);
		PrimaryActorTick.bCanEverTick = false;
		SetActorEnableCollision(false);
		UE_LOG(LogTemp, Warning, TEXT("still dead"));
	}
}
