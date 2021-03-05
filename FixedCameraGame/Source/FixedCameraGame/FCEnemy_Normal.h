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

	void StartDead(int32 currentReviveTime, int32 reviveCount);

	UPROPERTY(VisibleAnywhere, Category = "Revive")
		int32 reviveTime;
	UPROPERTY(VisibleAnywhere, Category = "Revive")
		int32 reviveCounter = 0;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool turning; 

};
