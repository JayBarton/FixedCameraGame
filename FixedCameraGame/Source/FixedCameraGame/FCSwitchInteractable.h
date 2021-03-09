// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCSwitchInteractable.generated.h"

/**
 * 
 */

class UFCSwitchComponent;

UCLASS()
class FIXEDCAMERAGAME_API AFCSwitchInteractable : public AFCInteractable
{
	GENERATED_BODY()
public:

	AFCSwitchInteractable();

	virtual void Action_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void PressSwitch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
		bool isToggle = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString inactiveText = "ph";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UFCSwitchComponent* Switch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable")
		FString buttonPrompt = "ph";

};
