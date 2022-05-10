// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCDeadWeapon.generated.h"

UCLASS()
class FIXEDCAMERAGAME_API AFCDeadWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCDeadWeapon();
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkeletalMeshComponent* MeshComp;
};
