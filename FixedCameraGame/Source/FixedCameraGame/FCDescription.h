// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCDescription.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCDescription : public AFCInteractable
{
	GENERATED_BODY()
public:
	virtual void Action_Implementation() override;
};