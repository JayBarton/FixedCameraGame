// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzlePawnBase.h"
#include "FCLightsPuzzle.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCLightsPuzzle : public AFCPuzzlePawnBase
{
	GENERATED_BODY()
public:
	AFCLightsPuzzle();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
	void ToggleLight();

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


	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		TArray<bool> grid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TArray<bool> gridDefault;
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		int32 index;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		int32 size = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		FVector startLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TArray<AActor*> boxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* onSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* offSound = nullptr;
};
