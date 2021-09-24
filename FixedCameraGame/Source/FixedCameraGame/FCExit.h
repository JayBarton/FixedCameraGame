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
	//starting index of the next level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	int index;
	//starting index of the next level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	int cameraIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	FName levelName;

	//if the music of the current level continues in the next one
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	bool continueMusic;

	//index of this exit. Used for one way locks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current Level")
	int32 currentIndex = 0;
};
