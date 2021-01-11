// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "FCInventoryComponent.generated.h"


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
	bool AddToInventory(FItemStruct newItem);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void RemoveFromInventory(int index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<FItemStruct> inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		int32 capacity;
};
