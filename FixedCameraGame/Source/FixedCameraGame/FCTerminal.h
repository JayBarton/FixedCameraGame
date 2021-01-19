// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCInteractable.h"
#include "FCTerminal.generated.h"

/**
 * 
 */

class UWidgetComponent;
class UCameraComponent;
class ACameraActor;
class UFCLockComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCTerminal : public AFCInteractable
{
	GENERATED_BODY()

public:
	AFCTerminal();
	virtual void Action_Implementation() override;

	//Sends the password to the widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void SendToWidget();

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void OpenLock();

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void ExitTerminal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	FString password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terminal")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ACameraActor* playerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		AFCInteractable* linkedInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		UFCLockComponent* linkedLock;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
