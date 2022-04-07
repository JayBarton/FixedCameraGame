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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(int32 damage) override;
	virtual void FinishAttack() override;

	virtual void TakeDamage(int32 damageAmount, FHitResult Hit) override;

	void ResumeAttack();

	UPROPERTY(BluePrintReadOnly, Category = "Attack")
	bool isCharging = false;

	UPROPERTY(BluePrintReadOnly, Category = "Attack")
	bool attackCoolingDown = false;

	//used to cancel out of the attack cool down
	bool beenHit = false;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	int32 chargeAttackDamage = 40;

	//Lower range for attack cooldown
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float coolDownTimeLower = 1.5f;
	//Upper range for attack cooldown
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float coolDownTimeLowerUpper = 3.0f;

	float timeSinceLastAttack = 0.0f;
	//Time from last attack until boss charges
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float chargeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float walkSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float chargeSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float walkTurnSpeed = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float chargeTurnSpeed = 20.0f;

	FTimerHandle CoolDownTimerHandle;

};
