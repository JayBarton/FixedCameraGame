// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCPlayer.generated.h"

#define COLLISION_PLAYER        ECC_GameTraceChannel2
#define COLLISION_INTERACT      ECC_GameTraceChannel3

class UFCInventoryComponent;
class AFCSwitchInteractable;
class UDataTable;
class UPawnNoiseEmitterComponent;
class AFCWeapon;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractableDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewInteractableDelegate);


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

	void QuickTurn();

	void StopSprinting();

	void Aim();

	void StopAiming();

	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void EquipWeapon(int32 inventoryIndex);
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void UnEquipWeapon(int32 inventoryIndex);

	void Interact();

	void OpenInventory();

	void StartQuickTurn();

	void StopQuickTurning();

	void LookForInteractable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
	void CreatePickUpWidget(AActor* pickup);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
	void CreateSwitchWidget(AFCSwitchInteractable* switchInteractable);

	//Display non-interactable inventory
	//May end up rewriting a lot of UI stuff
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
	void DisplayInventoryWidget();
	//Definitely rewriting some of this
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
	void ClearInventoryWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
	void Toggle(int32 mode, UFCLockComponent* lock, UFCInventoryComponent* containerInventory);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	bool UseKey(int32 index, UFCLockComponent* lock);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SwapItems(int32 first, int32 second);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SwapWithContainer(int32 first, int32 second, UFCInventoryComponent* containerInventory);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void Heal(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void CombineItems(int32 first, int32 second);

	void Reload();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void FinishReloading();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void ReloadWeapon(int32 first, int32 second);
	
	//Not sure if this is needed, seems a little superfluous
	//It works, I can simplify things later if I want
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void TakeHit(int32 damage);

	UFUNCTION()
	void RecoverFromStagger();
	UFUNCTION()
	void ResetLevel();
	UFUNCTION(BlueprintCallable, Category = "Functions")	
	bool BlockingInput();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void FootstepSound();

	//Just using this to test the text display, come up with a better solution later
	//Still want a better solution
	UPROPERTY(BlueprintReadWrite, Category = "Delete Later")
		bool inControl = true;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool inInventory = false;
	bool aimButtonDown = false;
	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	bool isAiming = false;
	UPROPERTY(BlueprintReadWrite, Category = "Weapons")
	bool isReloading = false;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool staggered = false;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool dead = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool walkingBackwards = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool quickTurn = false;
	
	FRotator reversedDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float walkSpeed = 125.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float runSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int32 maxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	int32 currentHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Weapons")
	int32 equipped = -1;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		FName WeaponAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		AFCWeapon* currentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TSubclassOf<AFCWeapon> weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* footstep;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TMap< TEnumAsByte<EPhysicalSurface>, int32> surfaceStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UFCInventoryComponent* Inventory;
	//I don't know that it's a great solution to have a reference to the item data here,
	//I need it for the item combination, it will work for now.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* itemData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* nearestInteractable;

	//Probably going to rework this later
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> pWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	UUserWidget* pickupWidget = nullptr;

	FTimerHandle LookTimerHandle;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FInteractableDelegate Interacted;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FNewInteractableDelegate NewInteractable;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
//	TSubclassOf<UUserWidget> inventoryUI = nullptr;

};
