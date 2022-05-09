// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCGameOverSequenceActor.generated.h"
class AFCPlayerCamera;
class ASpotLight;
UCLASS()
class FIXEDCAMERAGAME_API AFCGameOverSequenceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCGameOverSequenceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FadeOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	AFCPlayerCamera* camera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float distanceFromCenter = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float rotationSpeed = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float currentRotation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float transitionTime = 1.0f;
	float transitionTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float textDisplayTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	ASpotLight* fadeOutLight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float coneDefault = 44.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	float coneDescreaseSpeed = 1.0f;
	float currentCone;


	bool levelFadeIn = false;
	bool levelFadeOut = false;
};
