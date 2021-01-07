// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCContainer.generated.h"

/**
 * 
 */
class UFCInventoryComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCContainer : public AFCInteractable
{
	GENERATED_BODY()
private:

	AFCContainer();

public:
	//virtual void Action_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		UFCInventoryComponent* Inventory;
};