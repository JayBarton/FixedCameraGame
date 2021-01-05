// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCPlayer.generated.h"

class UFCInventoryComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFCPlayer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void Turn(float Value);

	void Sprint();

	void StopSprinting();

	void Interact();

	void LookForInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float walkSpeed = 125.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float runSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		UFCInventoryComponent* Inventory;

	AActor* nearestInteractable;

	FTimerHandle LookTimerHandle;

};
