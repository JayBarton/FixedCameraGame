// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCCameraSwitchVolume.generated.h"

class UBoxComponent;
class UTextRenderComponent;
class AFCPlayerCamera;

UCLASS()
class FIXEDCAMERAGAME_API AFCCameraSwitchVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCCameraSwitchVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform);

	UFUNCTION()
	void OnEndOverlap(AActor* thisActor, AActor* otherActor);

	UBoxComponent* boxCollison;

	UTextRenderComponent* textRenderNewCamera;
	UTextRenderComponent* textRenderPreviousCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volume")
	float width = 32.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volume")
	float height = 32.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
		ACameraActor* newCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
		ACameraActor* previousCamera;
};
