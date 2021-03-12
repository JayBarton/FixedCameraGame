// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy_Normal.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"

#include "Kismet/GameplayStatics.h"

AFCEnemy_Normal::AFCEnemy_Normal()
{
}

void AFCEnemy_Normal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!spawnIn && !spawning)
	{
		if (canRevive)
		{
			FVector2D currentPosition(GetActorLocation().X, GetActorLocation().Y);
			FVector2D playerPosition(player->GetActorLocation().X, player->GetActorLocation().Y);
			float distance = (currentPosition - playerPosition).Size();
			if (distance < 300.0f)
			{
				canRevive = false;
				dead = false;
				SetActorEnableCollision(true);

				FTimerHandle ReviveTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(ReviveTimerHandle, this, &AFCEnemy_Normal::Revive, 0.60f, false);
			}
		}
		else if (!staggered)
		{
			if (hasNoticedPlayer)
			{

				//check if in range for attack
				FVector2D currentPosition(GetActorLocation().X, GetActorLocation().Y);
				FVector2D playerPosition(player->GetActorLocation().X, player->GetActorLocation().Y);
				float distance = (currentPosition - playerPosition).Size();
				if (distance < attackDistance)
				{
					UE_LOG(LogTemp, Warning, TEXT("U3"));

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
						//UE_LOG(LogTemp, Warning, TEXT("Attack"));
						//UE_LOG(LogTemp, Warning, TEXT("delat yaw %f"), abs(deltaYaw));
						isAttacking = true;
					}
				}
				else
				{
					UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), player);
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
	}
}

void AFCEnemy_Normal::Attack()
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
	if (GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, COLLISION_PLAYER, shape, CollisionParams))
	{
	//	UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

		UGameplayStatics::ApplyDamage(player, 20, GetInstigatorController(), this, UDamageType::StaticClass());
		//player->TakeDamage
		//player-TakeHit(20);
	}
}

void AFCEnemy_Normal::FinishAttack()
{
	Super::FinishAttack();
	NoticePlayer();
}

void AFCEnemy_Normal::Kill()
{
	Super::Kill();
	reviveTime = FMath::RandRange(3, 6); 
}

void AFCEnemy_Normal::StartDead(int32 currentReviveTime, int32 reviveCount)
{
	reviveTime = currentReviveTime;
	reviveCounter = reviveCount + 1;
	if(reviveCounter >= reviveTime)
	{
		dead = true;
		PawnSensingComp->SetSensingUpdatesEnabled(false);
		SetActorEnableCollision(false);
		canRevive = true;
		UE_LOG(LogTemp, Warning, TEXT("revive"));
	}
	else
	{
		Super::Kill();

		/*PawnSensingComp->SetSensingUpdatesEnabled(false);
		PrimaryActorTick.bCanEverTick = false;
		SetActorEnableCollision(false);*/
		UE_LOG(LogTemp, Warning, TEXT("still dead"));
	}
}

void AFCEnemy_Normal::Revive()
{
	UE_LOG(LogTemp, Warning, TEXT("reviving here"));
	reviveCounter = 0;
	PawnSensingComp->SetSensingUpdatesEnabled(true);
	SetActorTickEnabled(true);
	Super::NoticePlayer();
}
