// Copyright Epic Games, Inc. All Rights Reserved.

#include "FixedCameraGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

//////////////////////////////////////////////////////////////////////////
// AFixedCameraGameCharacter

AFixedCameraGameCharacter::AFixedCameraGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFixedCameraGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFixedCameraGameCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFixedCameraGameCharacter::StopSprinting);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFixedCameraGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &AFixedCameraGameCharacter::Turn);
}

void AFixedCameraGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	UE_LOG(LogTemp, Warning, TEXT("AAA"));
}

void AFixedCameraGameCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFixedCameraGameCharacter::Turn(float value)
{
	if (value != 0.0f)
	{
		//turnAmount = value;
		//FRotator rotation = FRotator(0.0f, turnAmount, 0.0f);
		//FRotator rotation = FRotator(0.0f, value * 5.0f, 0.0f);
		FRotator rotation = FRotator(0.0f, value, 0.0f);
		AddActorLocalRotation(rotation);

		//AddControllerYawInput(value);
	}
}

void AFixedCameraGameCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void AFixedCameraGameCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}
