// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy_Boss.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

void AFCEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	turnSpeed = walkTurnSpeed;

}

void AFCEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentPosition = GetActorLocation();
	FVector playerPosition = player->GetActorLocation();
	float distance = (currentPosition - playerPosition).Size();
	if (!attackCoolingDown && !isCharging)
	{
		timeSinceLastAttack += DeltaTime;
		if (timeSinceLastAttack >= chargeTime)
		{
			timeSinceLastAttack = 0.0f;
			isCharging = true;
			GetCharacterMovement()->MaxWalkSpeed = chargeSpeed;
			turnSpeed = chargeTurnSpeed;
		}
	}
	else if (attackCoolingDown)
	{
		if (beenHit)
		{
			GetWorld()->GetTimerManager().ClearTimer(CoolDownTimerHandle);
			ResumeAttack();
		}
	}

	beenHit = false;
}

void AFCEnemy_Boss::Attack(int32 damage)
{
	if (isCharging)
	{
		Super::Attack(chargeAttackDamage);
		isCharging = false;
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		turnSpeed = walkTurnSpeed;
	}
	else
	{
		Super::Attack(attackDamage);
	}
	timeSinceLastAttack = 0.0f;
}

void AFCEnemy_Boss::FinishAttack()
{
	isAttacking = false;
	PawnSensingComp->SetSensingUpdatesEnabled(false);
	attackCoolingDown = true;
	//How long after an attack until the boss begins moving again
	float coolDownTime = FMath::FRandRange(coolDownTimeLower, coolDownTimeLowerUpper);
	UE_LOG(LogTemp, Warning, TEXT("%f"), coolDownTime);
	GetWorld()->GetTimerManager().SetTimer(CoolDownTimerHandle, this, &AFCEnemy_Boss::ResumeAttack, coolDownTime, false);

}

void AFCEnemy_Boss::TakeDamage(int32 damageAmount, FHitResult Hit)
{
	Super::TakeDamage(damageAmount, Hit);
	beenHit = true;
}

void AFCEnemy_Boss::ResumeAttack()
{
	PawnSensingComp->SetSensingUpdatesEnabled(true);
	attackCoolingDown = false;

}
