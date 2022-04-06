// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCEnemy_Normal.h"
#include "FCEnemy_Boss.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCEnemy_Boss : public AFCEnemy_Normal
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(int32 damage) override;

	UPROPERTY(BluePrintReadOnly, Category = "Attack")
		bool isCharging = false;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		int32 chargeAttackDamage = 40;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		float chargeDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float walkSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float chargeSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float walkTurnSpeed = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float chargeTurnSpeed = 6.0f;
};
