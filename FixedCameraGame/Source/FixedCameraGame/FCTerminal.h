// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCPuzzleInteractable.h"
#include "FCTerminal.generated.h"

/**
 * 
 */

class UWidgetComponent;
class UCameraComponent;
class ACameraActor;
class UFCLockComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCTerminal : public AFCPuzzleInteractable
{
	GENERATED_BODY()

public:
	AFCTerminal();

	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckScreen();

	virtual void StartPuzzle();

	//Sends the password to the widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void SendToWidget();

		virtual void OpenLock() override;
	virtual void ExitPuzzle() override;

	UFUNCTION()
	void Unlocked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	FString password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
		UMaterialInterface* blankMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
		UMaterialInterface* onMaterial;
};
