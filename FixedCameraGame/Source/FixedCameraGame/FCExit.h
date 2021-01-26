// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCExit.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCExit : public AFCInteractable
{
	GENERATED_BODY()

public:
	AFCExit();
	virtual void Action_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	int index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	FName levelName;
};
