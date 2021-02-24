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

	void TakeDamage(int32 damageAmount);

	void Kill();

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

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float attackDistance = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float attackRange = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	int32 hp;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool dead;
	
	FRotator rotatorDirection;
};
