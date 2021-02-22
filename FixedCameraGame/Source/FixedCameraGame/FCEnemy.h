// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCEnemy.generated.h"

class UPawnSensingComponent;

UCLASS()
class FIXEDCAMERAGAME_API AFCEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFCEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetUpAttack();

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volumne);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void NoticePlayer();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void Attack();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComp;

	APawn* player = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool hasNoticedPlayer = false;
	//playing attack animation
	//read write should be temporary
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	bool isAttacking = false;
	//for the actual attack damage
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool hasAttacked = false;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool turning;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float attackDistance = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float attackRange = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float attackLength = 1.7f;
	float halfAttackLength = 0.0f;
	float attackTime = 0.0f;

	FRotator rotatorDirection;

};
