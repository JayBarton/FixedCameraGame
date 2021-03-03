// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy_Normal.h"
#include "DrawDebugHelpers.h"

AFCEnemy_Normal::AFCEnemy_Normal()
{
	canRevive = true;
}

void AFCEnemy_Normal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!staggered)
	{
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
					//UE_LOG(LogTemp, Warning, TEXT("Attack"));
					//UE_LOG(LogTemp, Warning, TEXT("delat yaw %f"), abs(deltaYaw));
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
}

void AFCEnemy_Normal::FinishAttack()
{
	Super::FinishAttack();
	NoticePlayer();
}
