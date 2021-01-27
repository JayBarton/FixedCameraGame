// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCDescription.generated.h"

/**
 * 
 */
class UFCInfoTextWidget;
UCLASS()
class FIXEDCAMERAGAME_API AFCDescription : public AFCInteractable
{
	GENERATED_BODY()
public:
	AFCDescription();

	virtual void Action_Implementation() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString description;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UFCInfoTextWidget> infoWidget;
};
