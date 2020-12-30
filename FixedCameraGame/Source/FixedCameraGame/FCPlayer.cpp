// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AFCPlayer::AFCPlayer()
{	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFCPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

}

// Called every frame
void AFCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFCPlayer::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFCPlayer::StopSprinting);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFCPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &AFCPlayer::Turn);

}

void AFCPlayer::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFCPlayer::Turn(float value)
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

void AFCPlayer::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;

}

void AFCPlayer::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}
