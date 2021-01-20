// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCPuzzleIneractable.generated.h"

class ACameraActor;
class UFCLockComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCPuzzleIneractable : public AFCInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCPuzzleIneractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Action_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Functions") //override
		void OpenLock();

	UFUNCTION(BlueprintCallable, Category = "Functions") //override
		virtual void ExitPuzzle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player") //
		ACameraActor* playerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level") 
		AFCInteractable* linkedInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level") 
		UFCLockComponent* linkedLock;
};
