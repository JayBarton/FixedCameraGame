// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCPlayer.generated.h"

class UFCInventoryComponent;
class AFCSwitchInteractable;
class UDataTable;
class UPawnNoiseEmitterComponent;


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

	void Sprint();

	void StopSprinting();

	void Interact();

	void OpenInventory();

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
		void Heal(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void CombineItems(int32 first, int32 second);

	//Just using this to test the text display, come up with a better solution later
	//Still want a better solution
	UPROPERTY(BlueprintReadWrite, Category = "Delete Later")
		bool inControl = true;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
		bool inInventory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float walkSpeed = 125.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float runSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 maxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 currentHealth;

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
