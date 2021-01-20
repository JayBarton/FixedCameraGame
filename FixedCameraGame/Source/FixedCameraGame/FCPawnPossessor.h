// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzleIneractable.h"
#include "FCPawnPossessor.generated.h"

/**
 * 
 */
class ACameraActor;
class AFCPuzzlePawnBase;
UCLASS()
class FIXEDCAMERAGAME_API AFCPawnPossessor : public AFCPuzzleIneractable
{
	GENERATED_BODY()
public:
	AFCPawnPossessor();
	virtual void Action_Implementation() override;

	virtual void ExitPuzzle() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	AFCPuzzlePawnBase* puzzlePawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	ACameraActor* newCamera;
	APawn* playerPawn;
	ACameraActor* playerCamera;

};
