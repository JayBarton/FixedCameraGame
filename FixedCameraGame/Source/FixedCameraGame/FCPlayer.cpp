// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "FCInteractableInterface.h"
#include "FCInventoryComponent.h"
#include "FCLockComponent.h"
#include "Blueprint/UserWidget.h" 
#include "FCInteractable.h"
#include "FCExit.h"
#include "FCWeapon.h"
#include "Kismet/GameplayStatics.h" 
#include "FixedCameraGameGameMode.h"
#include "Engine/DataTable.h" 
#include "Components/PawnNoiseEmitterComponent.h"
#include "FCMultiLockComponent.h"
#include "FCFootStepComponent.h"
#include "FCSwitchComponent.h"

#include "GameFramework/PlayerInput.h" 
#include "DrawDebugHelpers.h" 

// Sets default values
AFCPlayer::AFCPlayer()
{	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Block);


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	Inventory = CreateDefaultSubobject<UFCInventoryComponent>(TEXT("Inventory"));
	Inventory->capacity = 6;

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComponent"));

	FootStepComponent = CreateDefaultSubobject<UFCFootStepComponent>(TEXT("FootStepComponent"));

	currentHealth = maxHealth;
	OnTakeAnyDamage.AddDynamic(this, &AFCPlayer::HandleTakeAnyDamage);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFCPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetWorld()->GetTimerManager().SetTimer(LookTimerHandle, this, &AFCPlayer::LookForInteractable, 0.2f, true);

	//Init();
	
	//GetWorld()->GetTimerManager().PauseTimer(LookTimerHandle);
//	GetWorld()->GetTimerManager().
	//LookTimerHandle.Invalidate
	
}

void AFCPlayer::Init()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	currentWeapon = GetWorld()->SpawnActor<AFCWeapon>(weapon, SpawnInfo);
	currentWeapon->player = this;
	currentWeapon->Hide();

	if (equipped >= 0)
	{
		EquipWeapon(equipped);
	}
}

// Called every frame
void AFCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (quickTurn)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), reversedDirection, DeltaTime, 10.0f));
		//FRotator rotation = FRotator(0.0f, 5.0f, 0.0f);
		//AddActorLocalRotation(rotation);
		if (GetActorRotation().Equals(reversedDirection, 5.0f))
		{
			SetActorRotation(reversedDirection);
			StopQuickTurning();
		}
	}
	if (aimButtonDown)
	{
		//Similar to sprinting, this is the best way I can find to do this.
		auto pc = Cast<APlayerController>(GetController());
		auto aimButtons = pc->PlayerInput->GetKeysForAction("Aim");
		if (!pc->IsInputKeyDown(aimButtons[0].Key) && !pc->IsInputKeyDown(aimButtons[1].Key))
		{
			StopAiming();
		}
		else
		{
			isAiming = true;
		}
	}
	else
	{
		auto pc = Cast<APlayerController>(GetController());
		auto aimButtons = pc->PlayerInput->GetKeysForAction("Aim");
		if (pc->IsInputKeyDown(aimButtons[0].Key) || pc->IsInputKeyDown(aimButtons[1].Key))
		{
			Aim();
		}
		else
		{
			isAiming = false;
		}
	}

}

// Called to bind functionality to input
void AFCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFCPlayer::QuickTurn);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFCPlayer::Fire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFCPlayer::Reload);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCPlayer::Interact);

	//PlayerInputComponent->BindAction("Open Inventory", IE_Pressed, this, &AFCPlayer::OpenInventory);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFCPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &AFCPlayer::Turn);

}

void AFCPlayer::MoveForward(float value)
{
	if (!BlockingInput())
	{
		walkingBackwards = false;
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

		if (!isAiming && !isReloading)
		{
			if (value != 0.0f)
			{
				if (value < 0)
				{
					walkingBackwards = true;
				}
				else
				{
					//This is the only way I could find to see if a button is held down
					//Clean up later. Will get the keys up in setup and save them
					//Should only ever have 2 binds, for keyboard and controller.
					auto pc = Cast<APlayerController>(GetController());
					auto sprintButtons = pc->PlayerInput->GetKeysForAction("Sprint");
					if(pc->IsInputKeyDown(sprintButtons[0].Key) || pc->IsInputKeyDown(sprintButtons[1].Key))
					{
						GetCharacterMovement()->MaxWalkSpeed = runSpeed;
					}
				}
				AddMovementInput(GetActorForwardVector(), value);
			}
		}
	}
}

void AFCPlayer::Turn(float value)
{
	if (!BlockingInput())
	{
		if (value != 0.0f)
		{
			//turnAmount = value;
			FRotator rotation = FRotator(0.0f, value * GetWorld()->GetDeltaSeconds() * 100, 0.0f);
			//FRotator rotation = FRotator(0.0f, value, 0.0f);
			AddActorLocalRotation(rotation);

			//AddControllerYawInput(value);
		}
	}
}

void AFCPlayer::QuickTurn()
{
	if (!BlockingInput())
	{
		if (walkingBackwards)
		{
			StartQuickTurn();
		}
	}
}

//Don't think I'm using this anymore
void AFCPlayer::StopSprinting()
{
//	if (!BlockingInput())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hel"));
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
}

void AFCPlayer::Aim()
{
	if (equipped >= 0)
	{
		//Can't interact while aiming, so turn off the timer
		nearestInteractable = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(LookTimerHandle);
		aimButtonDown = true;	
		//isAiming = true;
	}
}

void AFCPlayer::StopAiming()
{
	if (equipped >= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(LookTimerHandle, this, &AFCPlayer::LookForInteractable, 0.2f, true);
		aimButtonDown = false;
		//isAiming = false;
	}
}

void AFCPlayer::Fire()
{
	if (!BlockingInput())
	{
		if (isAiming && !isReloading && currentWeapon->canFire)
		{
			FItemStruct& weaponSlot = Inventory->inventory[equipped];
			if (weaponSlot.amount > 0)
			{
				weaponSlot.amount--;
				currentWeapon->Fire();
				MakeNoise(2.0f, this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("reloading"));
				Reload();
			}
		}
	}
}

void AFCPlayer::EquipWeapon(int32 inventoryIndex)
{
	FItemStruct& slot = Inventory->inventory[inventoryIndex];
	equipped = inventoryIndex;
	currentWeapon->ammoID = slot.ammoID;
	currentWeapon->Show();
	//currentWeapon->currentAmmo = slot.amount;
	//currentWeapon->currentAmmo = slot.amount;
}

void AFCPlayer::UnEquipWeapon(int32 inventoryIndex)
{
	equipped = -1;
	currentWeapon->Hide();
	//FItemStruct& slot = Inventory->inventory[inventoryIndex];
	//slot.amount = currentWeapon->currentAmmo;
}

void AFCPlayer::Interact()
{
	if (!BlockingInput() && !isAiming)
	{
		if (nearestInteractable)
		{
			if (auto lock = Cast<UFCLockComponent>(nearestInteractable->FindComponentByClass(UFCLockComponent::StaticClass())))
			{
				auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				//check lock type
				if (lock->type == LockType::PUZZLE)
				{
					gameMode->DisplayText(lock->description);
				}
				else if (lock->type == LockType::KEY || lock->type == LockType::PUZZLE)
				{
					if (lock->lockedSound)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), lock->lockedSound);
					}
					gameMode->DisplayText(lock->description, lock);
				}
				else if (lock->type == LockType::ONE_WAY_LOCKED)
				{
					gameMode->DisplayText("Locked from the other side.");
					if (lock->lockedSound)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), lock->lockedSound);
					}

				}
				else if (lock->type == LockType::ONE_WAY_UNLOCK)
				{
					auto exit = Cast<AFCExit>(nearestInteractable);
					gameMode->SetPendingLock(exit->levelName.ToString(), exit->index);
					lock->Open(lock->ID);

					gameMode->DisplayText("Unlocked");
				}
			}
			else if (auto multiLock = Cast<UFCMultiLockComponent>(nearestInteractable->FindComponentByClass(UFCMultiLockComponent::StaticClass())))
			{
				if (multiLock->CheckLocks())
				{
					//I don't believe this branch will ever fire...
					IFCInteractableInterface::Execute_Action(nearestInteractable);
				}
				else
				{
					//play lock sound
					auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					gameMode->DisplayText(multiLock->description);
				}
			}
			else
			{
				IFCInteractableInterface::Execute_Action(nearestInteractable);
			}
			Interacted.Broadcast();
		}
	}
}

void AFCPlayer::OpenInventory()
{
	/*
	 UserInterface = CreateWidget<UMyUserWidget>(this, UMyUserWidget::StaticClass());
	 FInputModeGameAndUI Mode;
	 Mode.SetLockMouseToViewport(true);
	 Mode.SetHideCursorDuringCapture(false);
	 SetInputMode(Mode);
	 UserInterface->AddToViewport(9999); // Z-order, this just makes it render on the very top.
	*/
/*	if (inventoryUI)
	{
		auto bedroomEyes = CreateWidget<UUserWidget>(GetWorld(), inventoryUI);
		bedroomEyes->AddToViewport();
	}*/
}

void AFCPlayer::StartQuickTurn()
{
	reversedDirection = (GetActorForwardVector() * -1).Rotation();
	quickTurn = true;
//	DisableInput(Cast<APlayerController>(GetController()));
}

void AFCPlayer::StopQuickTurning()
{
//	EnableInput(Cast<APlayerController>(GetController()));
	quickTurn = false;
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

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, COLLISION_INTERACT, CollisionParams))
	{
		//Need to do this first getactor check because I get a crash when the line intersects with box brushes
		//If/when I get rid of box brushes, I can get rid of this
		if (OutHit.GetActor())
		{
			AActor* hitActor = OutHit.GetActor();

			if (hitActor->GetClass()->ImplementsInterface(UFCInteractableInterface::StaticClass()))
			{
				interactable = true;
				if (!nearestInteractable)
				{
					nearestInteractable = hitActor;
				}
				else
				{
					if (hitActor != nearestInteractable)
					{

						float d1 = (Start - hitActor->GetActorLocation()).Size();
						float d2 = (Start - nearestInteractable->GetActorLocation()).Size();
						if (d1 < d2)
						{
							nearestInteractable = hitActor;
						}
					}
				}
			}
		}
	}
	if (!interactable)
	{
		if (nearestInteractable)
		{
			nearestInteractable = nullptr;
		}
	}
}

bool AFCPlayer::UseKey(int32 index, UFCLockComponent* lock)
{
	int32 ID = Inventory->inventory[index].ID;
	if (ID == lock->ID)
	{
		Inventory->RemoveFromInventory(index);
		lock->Open(ID);

		return true;
	}
	return false;
}

void AFCPlayer::SwapItems(int32 first, int32 second)
{
	if (equipped >= 0)
	{
		if (equipped == first)
		{
			equipped = second;
		}
		else if (equipped == second)
		{
			equipped = first;
		}
	}
	FItemStruct temp = Inventory->inventory[second];
	Inventory->inventory[second] = Inventory->inventory[first];
	Inventory->inventory[first] = temp;
}

void AFCPlayer::SwapWithContainer(int32 first, int32 second, UFCInventoryComponent* containerInventory)
{
	if (equipped == first)
	{
		UnEquipWeapon(first);
	}
	FItemStruct temp = containerInventory->inventory[second];
	containerInventory->inventory[second] = Inventory->inventory[first];
	Inventory->inventory[first] = temp;
}

void AFCPlayer::Heal(int32 index)
{
	currentHealth += Inventory->inventory[index].healAmount;
	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
	Inventory->RemoveFromInventory(index);
}

void AFCPlayer::CombineItems(int32 first, int32 second)
{
	FItemStruct& firstItem = Inventory->inventory[first];
	FItemStruct& secondItem = Inventory->inventory[second];
	bool combined = false;
	if (firstItem.combineWith == secondItem.ID)
	{
		firstItem.ID = firstItem.combineResult;
		if (itemData)
		{
			auto toLoad = itemData->FindRow<FItemStruct>(FName(*FString::FromInt(firstItem.ID)), "");
			if (toLoad)
			{
				firstItem = *toLoad;
			}
		}
		secondItem = FItemStruct();
		//update slots (in blueprint)
	}
	else
	{
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->DisplayText("Cannot combine these");
	}
}

void AFCPlayer::Reload()
{
	if (isAiming && !isReloading && Inventory->inventory[equipped].amount < currentWeapon->maxAmmo)
	{
		FItemStruct& weaponSlot = Inventory->inventory[equipped];
		for (int i = 0; i < Inventory->inventory.Num(); i++)
		{
			FItemStruct& slot = Inventory->inventory[i];
			if (slot.ID == currentWeapon->ammoID)
			{
				isReloading = true;
				break;
			}
		}
		if(!isReloading)
		{
			UE_LOG(LogTemp, Warning, TEXT("No ammo"));
		}
	}
}

void AFCPlayer::FinishReloading()
{
	FItemStruct& weaponSlot = Inventory->inventory[equipped];

	int toAdd = currentWeapon->maxAmmo - weaponSlot.amount;
	UE_LOG(LogTemp, Warning, TEXT("to add %i"), toAdd);
	UE_LOG(LogTemp, Warning, TEXT("ammo id %i"), currentWeapon->ammoID);
	int canAdd = 0;
	for (int i = 0; i < Inventory->inventory.Num(); i++)
	{
		FItemStruct& slot = Inventory->inventory[i];
		if (slot.ID == currentWeapon->ammoID)
		{
			if (slot.amount <= toAdd)
			{
				canAdd += slot.amount;
				slot = FItemStruct();
			}
			else
			{
				canAdd += toAdd;
				slot.amount -= toAdd;
			}
			if (canAdd == toAdd)
			{
				//done
				break;
			}
			else
			{
				toAdd -= canAdd;
			}
		}
	}
	weaponSlot.amount += canAdd;
	isReloading = false;

}

void AFCPlayer::ReloadWeapon(int32 first, int32 second)
{
	FItemStruct& firstItem = Inventory->inventory[first];
	FItemStruct& secondItem = Inventory->inventory[second];
	if (!firstItem.isEquipable)
	{
		FItemStruct& temp = secondItem;
		secondItem = firstItem;
		firstItem = secondItem;
	}

	if (firstItem.ammoID == secondItem.ID)
	{
		int32 toAdd = firstItem.maxCapacity - firstItem.amount;
		if (secondItem.amount <= toAdd)
		{
			UE_LOG(LogTemp, Warning, TEXT("q1"));

			firstItem.amount += secondItem.amount;
			secondItem = FItemStruct();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%i"), toAdd);
			UE_LOG(LogTemp, Warning, TEXT("%i"), firstItem.amount);
			UE_LOG(LogTemp, Warning, TEXT("%i"), secondItem.amount);

			firstItem.amount += toAdd;
			secondItem.amount -= toAdd;
		}
	}
	else
	{
		auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->DisplayText("Cannot combine these");
	}

}

void AFCPlayer::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	TakeHit(Damage);
}

void AFCPlayer::TakeHit(int32 damage)
{
	if (!staggered && !dead)
	{
		quickTurn = false;
		isReloading = false;
		//StopAiming();
		UE_LOG(LogTemp, Warning, TEXT("Ow!"));
		currentHealth -= damage;
		if (currentHealth <= 0)
		{
			DisableInput(Cast<APlayerController>(GetController()));
			dead = true;
			//Not sure how I want to handle death yet, this will work as a proof of concept
			FTimerHandle GameOverTimer;
			GetWorld()->GetTimerManager().SetTimer(GameOverTimer, this, &AFCPlayer::GameOver, 2.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("dead!"));
			UGameplayStatics::PlaySound2D(GetWorld(), deathSound);
		}
		else
		{
			staggered = true;
			FTimerHandle StaggerTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(StaggerTimerHandle, this, &AFCPlayer::RecoverFromStagger, 0.5f, false);
			UGameplayStatics::PlaySound2D(GetWorld(), hurtSound);

		}
	}
}

void AFCPlayer::RecoverFromStagger()
{
	//EnableInput(Cast<APlayerController>(GetController()));
	staggered = false;
}

void AFCPlayer::GameOver()
{
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->EndGame();
}

bool AFCPlayer::BlockingInput()
{
	return staggered || quickTurn || !inControl;
}

void AFCPlayer::CreateSwitchWidget_Implementation(AFCSwitchInteractable* switchInteractable)
{
}

void AFCPlayer::CreatePickUpWidget_Implementation(AActor* pickup)
{
}

void AFCPlayer::DisplayInventoryWidget_Implementation()
{
}

void AFCPlayer::ClearInventoryWidget_Implementation()
{
}

void AFCPlayer::Toggle_Implementation(int32 mode, UFCLockComponent* lock, UFCInventoryComponent* containerInventory)
{
}

void AFCPlayer::CreateSaveWidget_Implementation(int token, int playerStartIndex)
{
}