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
	virtual void Tick(float DeltaTime) override;
	virtual void Attack() override;
	virtual void FinishAttack() override;

};
