// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzlePawnBase.h"
#include "FCSlidingBlockPuzzle.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API AFCSlidingBlockPuzzle : public AFCPuzzlePawnBase
{
	GENERATED_BODY()
public:
	AFCSlidingBlockPuzzle();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
	void MoveBlock();
	int32 FindAdjacent();
	virtual void CheckSolution() override;
	
	virtual void StartPuzzle() override;
	virtual void ExitPuzzle() override;

	//Used to interact with the props in blueprint
	//might merge this with start puzzle
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void SetUp();
	//Also for prop interaction. If I think of another way of handling that, both of these can be removed
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void updateIndex();

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		TArray<int32> grid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TArray<int32> gridDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		int32 index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		int32 size = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		FVector startLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<AActor> prop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TArray<AActor*> boxes;
};
