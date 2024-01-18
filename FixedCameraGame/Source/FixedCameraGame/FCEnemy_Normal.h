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
	virtual void Attack(int32 damage) override;
	virtual void FinishAttack() override;
	virtual void Kill() override;

	virtual void TakeHit(int32 damageAmount, FHitResult Hit) override;

	void StartDead(int32 currentReviveTime, int32 reviveCount);
	void Revive();
	//Using a slight delay before reviving the enemy. This is to prevent the enemy from
	//reviving too quickly if they revive near the entrance the player entered from
	void DelayRevive();
	virtual void PlaySpawnIn() override;

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
	float fastTurnSpeed = 7.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* spawnSound;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool turning; 

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool turningAttack;

	bool canRevive;

};
