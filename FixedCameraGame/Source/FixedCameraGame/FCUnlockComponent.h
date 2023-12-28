// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FCUnlockComponent.generated.h"

class AFCInteractable;
class UFCLockComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCUnlockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCUnlockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Functions")
		void OpenLock();
/*	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		AFCInteractable* linkedInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		UFCLockComponent* linkedLock;*/
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		TArray<AFCInteractable*> linkedInteractables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		TArray<UFCLockComponent*> linkedLocks;
};
