// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FCPuzzlePawnBase.generated.h"

class ACameraActor;
class AFCPawnPossessor;
UCLASS()
class FIXEDCAMERAGAME_API AFCPuzzlePawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFCPuzzlePawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CheckSolution();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void StartPuzzle();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	virtual void ExitPuzzle();
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SolvePuzzle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TSubclassOf<AActor> prop;

	APawn* playerPawn;
	ACameraActor* playerCamera;
	AFCPawnPossessor* parent;

};
