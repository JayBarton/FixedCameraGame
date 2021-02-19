// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemyAIController.h"
#include "FCEnemyPathFollowingComponent.h"

AFCEnemyAIController::AFCEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFCEnemyPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}
