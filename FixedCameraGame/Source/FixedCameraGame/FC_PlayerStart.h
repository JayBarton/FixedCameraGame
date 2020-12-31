// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "FC_PlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFC_PlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
	int startIndex = 0;
};
