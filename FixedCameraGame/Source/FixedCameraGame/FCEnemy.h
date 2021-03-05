// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCEnemy.generated.h"

#define COLLISION_PLAYER        ECC_GameTraceChannel2


class UPawnSensingComponent;
class UParticleSystem;
class UBoxComponent;

//direction enemy was hit from
UENUM(BlueprintType)
enum class HitDirection : uint8 {
	NONE UMETA(DisplayName = "NONE"),
	FRONT UMETA(DisplayName = "FRONT"),
	BACK UMETA(DisplayName = "BACK"),
	LEFT UMETA(DisplayName = "LEFT"),
	RIGHT UMETA(DisplayName = "RIGHT"),
};

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

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volumne);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void NoticePlayer();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void Attack();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void FinishAttack();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void RecoverFromStagger();

	void TakeDamage(int32 damageAmount, FHitResult Hit);

	void Stagger();

	void ResetStaggerHits();

	void DetermineImpactDirection(FHitResult& Hit);

	virtual void Kill();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ImpactFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		TArray<FName> FrontImpactPoints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		TArray<FName> BackImpactPoints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		TArray<FName> LeftImpactPoints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		TArray<FName> RightImpactPoints;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		HitDirection hitDirection;

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

	//distance from the player before attacking
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float attackDistance = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float attackRange = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	int32 hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	bool dead;
	UPROPERTY(BlueprintReadOnly, Category = "Stagger")
	bool staggered = false;
	//not sure about these variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	bool spawnIn; //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	bool spawning = false; //
	bool canRevive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stagger")
	int32 hitsToStagger = 4;
	int32 staggerHits = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float turnSpeed = 2.5f;
	
	FTimerHandle StaggerTimerHandle;

	FRotator rotatorDirection; //
};
