// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"


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
	UE_LOG(LogTemp, Warning, TEXT("heard it all  before"));

	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AFCEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hasNoticedPlayer)
	{
		//check if in range for attack
		FVector2D currentPosition(GetActorLocation().X, GetActorLocation().Y);
		FVector2D playerPosition(player->GetActorLocation().X, player->GetActorLocation().Y);
		float distance = (currentPosition - playerPosition).Size();
		if (distance < attackDistance)
		{
			hasNoticedPlayer = false;
			if (AController* AI = GetController())
			{
				AI->StopMovement();
			}
			FVector direction = FVector(playerPosition, 0.0f) - FVector(currentPosition, 0.0f);

			rotatorDirection = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();
			float deltaYaw = (rotatorDirection - GetActorRotation()).Yaw;
			if (abs(deltaYaw) >= 45.0f)
			{
				turning = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Attack"));
				UE_LOG(LogTemp, Warning, TEXT("delat yaw %f"), abs(deltaYaw));
				isAttacking = true;
			}

		}
	}
	if (turning)
	{
		float deltaYaw = (rotatorDirection - GetActorRotation()).Yaw;
		if (abs(deltaYaw) >= 45.0f)
		{
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), rotatorDirection, DeltaTime, 10.0f));
		}
		else
		{
			turning = false;
			isAttacking = true;
		}
	}
}

void AFCEnemy::SetUpAttack()
{

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
	if (!hasNoticedPlayer && !isAttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("H2"));
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
			UE_LOG(LogTemp, Warning, TEXT("Get up"));
		}
	}
}

void AFCEnemy::Attack()
{
	FHitResult OutHit;

	FVector ForwardVector = GetActorForwardVector();
	FVector Start = GetActorLocation() + 80 * ForwardVector;

	FVector End = ((ForwardVector * attackRange) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	auto shape = FCollisionShape::MakeBox(FVector(100, 100, 100));
	DrawDebugBox(GetWorld(), Start, FVector(100, 100, 100), FColor::Red, false, 1.0f, 0.0f, 1.0f);
	DrawDebugBox(GetWorld(), End, FVector(100, 100, 100), FColor::Red, false, 1.0f, 0.0f, 1.0f);
}


void AFCEnemy::TakeDamage(int32 damageAmount, FHitResult Hit)
{

	float dot = FVector::DotProduct(Hit.ImpactNormal, GetActorForwardVector());

	if (dot == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("front"));
	}
	else if (dot == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("back"));
	}
	else
	{
		dot = FVector::DotProduct(Hit.ImpactNormal, GetActorRightVector());
		if (dot == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("right"));
		}
		else if (dot == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("left"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("dot %f"), dot);


	hp -= damageAmount;
	if (hp <= 0)
	{
		if (AController* AI = GetController())
		{
			AI->StopMovement();
		}
		Kill();
	}
}

void AFCEnemy::Kill()
{
	PawnSensingComp->SetSensingUpdatesEnabled(false);
	PrimaryActorTick.bCanEverTick = false;
	dead = true;
	SetActorEnableCollision(false);
}
