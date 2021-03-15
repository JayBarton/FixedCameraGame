// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCEnemy.h"
#include "FCEnemy_Normal.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCEnemy_Normal : public AFCEnemy
{
	GENERATED_BODY()
public:
	AFCEnemy_Normal();
	virtual void Tick(float DeltaTime) override;
	virtual void Attack() override;
	virtual void FinishAttack() override;
	virtual void Kill() override;

	virtual void TakeDamage(int32 damageAmount, FHitResult Hit) override;

	void StartDead(int32 currentReviveTime, int32 reviveCount);
	void Revive();

	int32 reviveTime;
	int32 reviveCounter = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Revive")
	int32 minReviveCount = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Revive")
	int32 maxReviveCount = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float defaultTurnSpeed = 2.5f;

	//use this to quickly turn around if hit in the back
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float fastTurnSpeed = 2.5f;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool turning; 

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool turningAttack;

	bool canRevive;

};
