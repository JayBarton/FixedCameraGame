// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FCLockComponent.generated.h"

//Used to handle interactables that can be locked, such as doors

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	void Open(int32 keyID);

	//The id that opens this lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
		FString description = "Door is locked";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
		FString unlockDescription = "Door unlocked";
		
};
