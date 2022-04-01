// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FCFootStepComponent.generated.h"

class USoundBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCFootStepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCFootStepComponent();

protected:

public:	
	UFUNCTION(BlueprintCallable, Category = "Functions")
		void FootstepSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* footstep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		TMap<TEnumAsByte<EPhysicalSurface>, int32> surfaceStep;
		
};
