// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "Structs.h"
#include "FCPickup.generated.h"

/**
 * 
 */
struct FItemStruct;
UCLASS()
class FIXEDCAMERAGAME_API AFCPickup : public AFCInteractable
{
	GENERATED_BODY()
public:
	AFCPickup();

	virtual void Action_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	int32 TakeItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	FItemStruct details;
};
