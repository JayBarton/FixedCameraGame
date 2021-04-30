// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCEnemySpawn.generated.h"

class UArrowComponent;
class UCapsuleComponent;
class UBillboardComponent;
class ATargetPoint;
class AFCEnemy;

UCLASS()
class FIXEDCAMERAGAME_API AFCEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCEnemySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<AFCEnemy> enemyType;
	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<ATargetPoint*> patrolPoints;

	//Using this for enemies that start out dead
	UPROPERTY(EditAnywhere, Category = "Enemy")
		int32 reviveCount = 0;
	
	UPROPERTY(BlueprintReadonly, Category = "Enemy")
	AFCEnemy* spawnedEnemy;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
		UBillboardComponent* ClickSprite;
	UPROPERTY()
		UArrowComponent* ArrowComponent;
#endif
};

