// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzlePawnBase.h"
#include "FCDotPuzzle.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDotArray
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		TArray<bool> row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<bool> rowDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<bool> solution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<AActor*> prop;
};

UCLASS()
class FIXEDCAMERAGAME_API AFCDotPuzzle : public AFCPuzzlePawnBase
{
	GENERATED_BODY()
public:
	AFCDotPuzzle();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	//void HandleAnimation(float DeltaTime);

	void MoveRight();
	void MoveLeft();
	void SelectSlot();
	void AxisX(float value);

	bool moveAxis = false;

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

	int32 numberOfSlots = 3;
	int32 currentSlot = 0;
	int32 numberOfRows = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TArray<FDotArray> slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* selectSound = nullptr;
};
