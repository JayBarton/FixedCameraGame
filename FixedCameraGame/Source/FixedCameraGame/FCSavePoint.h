// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCSavePoint.generated.h"

/**
 * 
 */
//It's possible that this could just be an entirely blueprint class. May remove this if that proves feasible.
UCLASS()
class FIXEDCAMERAGAME_API AFCSavePoint : public AFCInteractable
{
	GENERATED_BODY()
public:
	AFCSavePoint();
};
