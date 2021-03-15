// Fill out your copyright notice in the Description page of Project Settings.


#include "FCEnemyPathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "FCEnemy.h"

void UFCEnemyPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
    auto Owner = MovementComp->GetOwner();
    if (Owner)
    {
        if (!enemy)
        {
            enemy = Cast<AFCEnemy>(Owner);
        }

        FVector location = Owner->GetActorLocation();
        FVector target = Path->GetPathPoints()[GetNextPathIndex()].Location;
        FVector direction = target - location;
        FVector MoveVelocity = Owner->GetActorForwardVector() * CharacterMoveComp->MaxWalkSpeed;

        FVector desired = direction.GetSafeNormal() * CharacterMoveComp->MaxWalkSpeed;
        FVector steer = desired - MoveVelocity;

        float maxSteer = enemy->turnSpeed;

        if (steer.Size() > maxSteer)
        {
            steer.Normalize();
            steer *= maxSteer;
        }
        
        const int32 LastSegmentStartIndex = Path->GetPathPoints().Num() - 2;
        const bool bNotFollowingLastSegment = (MoveSegmentStartIndex < LastSegmentStartIndex);

        MoveVelocity = MoveVelocity + steer;

        if (MoveVelocity.Size() > CharacterMoveComp->MaxWalkSpeed)
        {
            MoveVelocity.Normalize();
            MoveVelocity *= CharacterMoveComp->MaxWalkSpeed;
        }
        PostProcessMove.ExecuteIfBound(this, MoveVelocity);
        MovementComp->RequestDirectMove(MoveVelocity, bNotFollowingLastSegment);
     //   UE_LOG(LogTemp, Warning, TEXT("velocity %s"), *MovementComp->Velocity.ToString());
       // UE_LOG(LogTemp, Warning, TEXT("steer %s"), *steer.ToString());
       // UE_LOG(LogTemp, Warning, TEXT("walk speed %f"), CharacterMoveComp->MaxWalkSpeed);
     /*   FRotator rotatorDirection = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();
        float deltaYaw = (rotatorDirection - Owner->GetActorRotation()).Yaw;
        if (deltaYaw >= 70.0f)
        {
            
            Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), rotatorDirection, DeltaTime, 2.0f));
           // UE_LOG(LogTemp, Warning, TEXT("%f"), deltaYaw);
        }
        else
        {
        }*/

    }
}

void UFCEnemyPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
    Super::SetMovementComponent(MoveComp);

    CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}
