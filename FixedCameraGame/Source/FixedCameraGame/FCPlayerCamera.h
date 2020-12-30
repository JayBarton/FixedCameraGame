// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FCPlayerCamera.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCPlayerCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AFCPlayerCamera();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool startingCamera;
	
};
