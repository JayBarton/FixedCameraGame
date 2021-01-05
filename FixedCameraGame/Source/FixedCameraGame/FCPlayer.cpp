// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "FCInteractableInterface.h"
#include "FCInventoryComponent.h"
#include "FCLockComponent.h"

#include "DrawDebugHelpers.h" 

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

	Inventory = CreateDefaultSubobject<UFCInventoryComponent>(TEXT("Inventory"));
	Inventory->capacity = 6;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFCPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	GetWorld()->GetTimerManager().SetTimer(LookTimerHandle, this, &AFCPlayer::LookForInteractable, 0.2f, true);
}

// Called every frame
void AFCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nearestInteractable)
	{
	}
}

// Called to bind functionality to input
void AFCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFCPlayer::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFCPlayer::StopSprinting);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCPlayer::Interact);

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
		//FRotator roton = FRotator(0.0f, turnAmount, 0.0f);
		//FRotator rotation 
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

void AFCPlayer::Interact()
{
	if (nearestInteractable)
	{
		if (auto lock = Cast<UFCLockComponent>(nearestInteractable->FindComponentByClass(UFCLockComponent::StaticClass())))
		{
			int id = -1;
			for (int i = 0; i < Inventory->inventory.Num(); i++)
			{
				if (Inventory->inventory[i].ID == lock->ID)
				{
					id = Inventory->inventory[i].ID;
					//remove from inventory
					//Would like to do this without shifting the array down...
					Inventory->RemoveFromInventory(i);
					break;
				}
			}
			lock->Open(id);
		}
		else
		{
			IFCInteractableInterface::Execute_Action(nearestInteractable);
		}
	}

}

void AFCPlayer::LookForInteractable()
{
	FHitResult OutHit;

	FVector Start = GetActorLocation();

	FVector ForwardVector = GetActorForwardVector();
	FVector End = ((ForwardVector * 100.0f) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, ECC_WorldStatic, 1.f);

	bool interactable = false;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.GetActor()->GetClass()->ImplementsInterface(UFCInteractableInterface::StaticClass()))
		{
			interactable = true;
			if (!nearestInteractable)
			{
				nearestInteractable = OutHit.GetActor();
				UE_LOG(LogTemp, Warning, TEXT("%s"), *nearestInteractable->GetName());

			}
		}

	}
	if (!interactable)
	{
		if (nearestInteractable)
		{
			UE_LOG(LogTemp, Warning, TEXT("nothin'"));
			nearestInteractable = nullptr;
		}
	}
}

