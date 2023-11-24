// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCPuzzleInteractable.generated.h"

class ACameraActor;
class UFCLockComponent;

class UFCSwitchComponent;
class UFCUnlockComponent;

UCLASS()
class FIXEDCAMERAGAME_API AFCPuzzleInteractable : public AFCInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCPuzzleInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Action_Implementation() override;

	virtual void StartPuzzle();

	UFUNCTION(BlueprintCallable, Category = "Functions") 
	virtual void OpenLock();

	UFUNCTION(BlueprintCallable, Category = "Functions") //override
	virtual void ExitPuzzle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString puzzleDescription = "ph";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString inactiveText = "ph";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FString puzzleCompleteText = "Door unlocked";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player") //
		ACameraActor* playerCamera;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level") 
		AFCInteractable* linkedInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level") 
		UFCLockComponent* linkedLock;*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UFCUnlockComponent* Unlocker;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UFCSwitchComponent* Switch;

};
