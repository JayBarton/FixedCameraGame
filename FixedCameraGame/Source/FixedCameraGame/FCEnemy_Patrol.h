// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCEnemy.h"
#include "FCEnemy_Patrol.generated.h"

class ATargetPoint;

UENUM(BlueprintType)
enum class PatrolState : uint8 {
	IDLE UMETA(DisplayName = "IDLE"),
	PATROL UMETA(DisplayName = "PATROL"),
	FOLLOWING UMETA(DisplayName = "FOLLOWING"),
	ATTACKING UMETA(DisplayName = "ATTACKING"),
};

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCEnemy_Patrol : public AFCEnemy
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void InitPath(TArray<ATargetPoint*> path);

	void Follow();

	void Patrol();

	virtual void NoticePlayer() override;

	virtual void Attack() override;
	virtual void FinishAttack() override;

	void MoveToNextPatrolPoint();
	
	void ResumeMovement();

	UPROPERTY(EditAnywhere)
	TArray<ATargetPoint*> patrolPoints;

	int currentTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
		PatrolState patrolState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float patrolSpeed = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float followSpeed = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float followTurnSpeed = 5.0f;
};
