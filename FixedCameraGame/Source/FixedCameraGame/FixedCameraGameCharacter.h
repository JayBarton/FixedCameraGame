// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FixedCameraGameCharacter.generated.h"

UCLASS(config=Game)
class AFixedCameraGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFixedCameraGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float turnAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float walkSpeed = 125.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float runSpeed = 500.0f;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	void Turn(float Value);

	void Sprint();
	void StopSprinting();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

};

