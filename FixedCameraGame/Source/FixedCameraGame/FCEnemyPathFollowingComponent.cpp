// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemyPathFollowingComponent.h"

void UFCEnemyPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
    auto Owner = MovementComp->GetOwner();
    if (Owner)
    {
        FVector location = Owner->GetActorLocation();
        FVector target = GetCurrentTargetLocation();
        FVector direction = target - location;

        FRotator rotatorDirection = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();
        float deltaYaw = (rotatorDirection - Owner->GetActorRotation()).Yaw;
        if (deltaYaw >= 70.0f)
        {
            
            Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), rotatorDirection, DeltaTime, 2.0f));
            UE_LOG(LogTemp, Warning, TEXT("%f"), deltaYaw);

        }
        else
        {
            Super::FollowPathSegment(DeltaTime);
        }

    }
    else
    {
        Super::FollowPathSegment(DeltaTime);

    }
}
