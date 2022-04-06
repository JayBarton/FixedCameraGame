// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemy_Boss.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void AFCEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentPosition = GetActorLocation();
	FVector playerPosition = player->GetActorLocation();
	float distance = (currentPosition - playerPosition).Size();
	//Not how this will work, just using this for simplicity
	if (!isCharging && distance > chargeDistance)
	{
		isCharging = true;
		GetCharacterMovement()->MaxWalkSpeed = chargeSpeed;
		turnSpeed = 25.0f;
	}
}

void AFCEnemy_Boss::Attack(int32 damage)
{
	if (isCharging)
	{
		Super::Attack(chargeAttackDamage);
		isCharging = false;
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		turnSpeed = 2.5f;
	}
	else
	{
		Super::Attack(attackDamage);
	}
}
