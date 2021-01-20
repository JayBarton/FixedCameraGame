// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzleIneractable.h"
#include "FCTerminal.generated.h"

/**
 * 
 */

class UWidgetComponent;
class UCameraComponent;
class ACameraActor;
class UFCLockComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCTerminal : public AFCPuzzleIneractable
{
	GENERATED_BODY()

public:
	AFCTerminal();
	virtual void Action_Implementation() override;

	//Sends the password to the widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void SendToWidget();

	virtual void ExitPuzzle() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	FString password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	UCameraComponent* Camera;

};
