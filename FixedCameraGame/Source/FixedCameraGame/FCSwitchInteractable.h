// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCSwitchInteractable.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchDelegate, bool, isOn);
/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCSwitchInteractable : public AFCInteractable
{
	GENERATED_BODY()
public:

	AFCSwitchInteractable();

	virtual void Action_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void PressSwitch();

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FSwitchDelegate Switch;
	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
		bool switchState = false;
};
