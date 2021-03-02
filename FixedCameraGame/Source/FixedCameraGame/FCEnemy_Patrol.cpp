// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy_Patrol.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h" 

void AFCEnemy_Patrol::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = patrolSpeed;
	if (patrolPoints.Num() > 0)
	{
		patrolState = PatrolState::PATROL;
		currentTarget = 0;
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), patrolPoints[currentTarget]);
	}
	else
	{
		patrolState = PatrolState::IDLE;
	}
}

void AFCEnemy_Patrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (patrolState)
	{
	case PatrolState::IDLE:
		break;
	case PatrolState::PATROL:
		Patrol();
		break;
	case PatrolState::FOLLOWING:
		Follow();
		break;
	case PatrolState::ATTACKING:
		AddMovementInput(GetActorForwardVector(), 1.0f);

		break;
	}
}

void AFCEnemy_Patrol::Follow()
{
	if (!staggered)
	{
		FVector2D currentPosition(GetActorLocation().X, GetActorLocation().Y);
		FVector2D playerPosition(player->GetActorLocation().X, player->GetActorLocation().Y);
		float distance = (currentPosition - playerPosition).Size();
		if (distance < attackDistance)
		{
			if (!isAttacking)
			{
				Attack();
			}
		}
		else
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), player);
		}
	}
}

void AFCEnemy_Patrol::Patrol()
{
	FVector Delta = GetActorLocation() - patrolPoints[currentTarget]->GetActorLocation();
	float DistanceToGoal = Delta.Size();

	//improve this
	if (DistanceToGoal < 55)
	{
		//int32 index = FMath::RandRange(0, 1);
		int32 index = 1;
		if (index == 0)
		{
			MoveToNextPatrolPoint();
		}
		else
		{
			patrolState = PatrolState::IDLE;
			FTimerHandle IdleTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &AFCEnemy_Patrol::ResumeMovement, 1.0f, false);
		}
	}
}

void AFCEnemy_Patrol::NoticePlayer()
{
	Super::NoticePlayer();

	patrolState = PatrolState::FOLLOWING;
	GetCharacterMovement()->MaxWalkSpeed = followSpeed;
	turnSpeed = followTurnSpeed;
	PawnSensingComp->SetSensingUpdatesEnabled(false);

}

void AFCEnemy_Patrol::Attack()
{
	isAttacking = true;
	patrolState = PatrolState::ATTACKING;

	AddMovementInput(GetActorForwardVector(), 1.0f);


}

void AFCEnemy_Patrol::FinishAttack()
{
	isAttacking = false;
	patrolState = PatrolState::FOLLOWING;
	Super::NoticePlayer();
}

void AFCEnemy_Patrol::MoveToNextPatrolPoint()
{
	currentTarget++;
	if (currentTarget >= patrolPoints.Num())
	{
		currentTarget = 0;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), patrolPoints[currentTarget]);

}

void AFCEnemy_Patrol::ResumeMovement()
{
	if (patrolState == PatrolState::IDLE)
	{
		patrolState = PatrolState::PATROL;
		MoveToNextPatrolPoint();
	}
}
