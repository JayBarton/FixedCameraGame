// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "FCInventoryComponent.generated.h"

class UUserWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Functions")
	int32 AddToInventory(FItemStruct newItem);

	int32 FindFreeSlot();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void RemoveFromInventory(int index);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<FItemStruct> inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		int32 capacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		UUserWidget* inventoryWindow;

};
