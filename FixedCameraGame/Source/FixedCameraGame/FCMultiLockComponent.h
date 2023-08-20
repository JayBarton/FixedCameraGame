// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FCMultiLockComponent.generated.h"

class UFCLockComponent;
class AFCInteractable;

USTRUCT(BlueprintType)
struct FLockStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locks")
	AFCInteractable* LockedActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locks")
	UFCLockComponent* Lock;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCMultiLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCMultiLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CheckLocks();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locks")
	TArray<FLockStruct> Locks;

	//Sound that plays there are still remaining locks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundBase* lockedSound = nullptr;
	//Sound that plays when all locks have been unlocked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundBase* unlockedSound = nullptr;

		
};
