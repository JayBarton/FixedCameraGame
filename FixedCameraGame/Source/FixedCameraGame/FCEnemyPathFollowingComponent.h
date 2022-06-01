// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "FCEnemyPathFollowingComponent.generated.h"

class UCharacterMovementComponent;
class AFCEnemy;
/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API UFCEnemyPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
protected:
	/** follow current path segment */
	virtual void FollowPathSegment(float DeltaTime) override;

	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;

public:
	UCharacterMovementComponent* CharacterMoveComp;
	AFCEnemy* enemy;

	float counter = 0.0f;
};
