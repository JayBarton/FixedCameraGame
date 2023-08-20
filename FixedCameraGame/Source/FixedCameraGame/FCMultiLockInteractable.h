// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCMultiLockInteractable.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCMultiLockInteractable : public AFCInteractable
{
	GENERATED_BODY()
public:
	AFCMultiLockInteractable();
	virtual void Action_Implementation() override;

	UFUNCTION()
	void SuperUniqueFunction(bool testing);

	UFUNCTION()
	void CheckDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UFCLockComponent* lockComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UFCSwitchComponent* switchComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class AFCExit* exit;

	bool unlocked = false;
};
