// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API UFCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//Index of the player start and camera on entering a new level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	int startIndex = 0;

};
