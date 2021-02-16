// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCSwitchInteractable.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchDelegate, bool, isOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchStartDelegate);
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
	//Using this to make sure all attached objects can remain in their "on" state
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSwitchStartDelegate SwitchOn;
	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
		bool switchState = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
		bool isToggle = true;
	//If we move the camera to show an effect, we don't want to return input until that effect is finished
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
		bool playScene = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString inactiveText = "ph";

};
