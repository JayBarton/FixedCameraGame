// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzlePawnBase.h"
#include "FCTowerPuzzle.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTowerArray
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<int32> disk;
};


UCLASS()
class FIXEDCAMERAGAME_API AFCTowerPuzzle : public AFCPuzzlePawnBase
{
	GENERATED_BODY()
public:
	AFCTowerPuzzle();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveRight();
	void MoveLeft();
	void SelectDisk();

	virtual void StartPuzzle() override;
	virtual void ExitPuzzle() override;
	virtual void CheckSolution() override;

	//Doing this in blueprint for now. Once I have a better idea of how the props will work I
	//may move it into C++
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void Toggle();
	//Same purpose as in the sliding block puzzle
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void SetUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void updateIndex();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void MoveDisk();

	int32 numberOfRods = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 currentRod;
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 selectedRod = -1;

	int32 numberOfDisks = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TArray<FTowerArray> towers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TArray<AActor*> rods;
};
