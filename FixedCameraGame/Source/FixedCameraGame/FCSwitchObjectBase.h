// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCSwitchObjectBase.generated.h"

class AFCSwitchInteractable;

UCLASS()
class FIXEDCAMERAGAME_API AFCSwitchObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCSwitchObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void SwitchAction(bool isOn);
	UFUNCTION()
		virtual void SetToOnState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		AFCSwitchInteractable* attachedSwitch;

};
