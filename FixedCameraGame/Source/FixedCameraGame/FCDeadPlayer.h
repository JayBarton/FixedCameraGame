// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCDeadPlayer.generated.h"

UCLASS()
class FIXEDCAMERAGAME_API AFCDeadPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCDeadPlayer();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapons")
		FName WeaponAttachPoint;
};
