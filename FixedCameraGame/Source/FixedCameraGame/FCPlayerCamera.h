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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	void SetDynamicMaterial();

	void UpdateMaterial(float t, float transitionTime);

	void SetMaterial(float alpha);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool startingCamera;

	//using this to allow the first camera to fade in
	bool materialSetUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start")
		int startIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
		UMaterialInterface* MaterialTest;

	UMaterialInstanceDynamic* dynamicMaterial;

};
