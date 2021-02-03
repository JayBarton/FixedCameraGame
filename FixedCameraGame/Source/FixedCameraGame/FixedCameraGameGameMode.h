// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FixedCameraGameGameMode.generated.h"

class UFCGameInstance;
class AFCObjectWatcher;
class AFCContainer;
class UFCInfoTextWidget;
class AFCPlayerCamera;

//Hopefully this being here is temporary
class UFCLockComponent;

DECLARE_DELEGATE_OneParam(InputDelegate, UFCLockComponent*);


UCLASS(minimalapi)
class AFixedCameraGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFixedCameraGameGameMode();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void CheckObjects(UFCGameInstance* instance);

	void FindStart(UFCGameInstance* instance);

	void ChangeLevel(int index, FName levelName);
	void MoveToLevel(FName levelName);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//I'm hoping I can find a better way to handle the locks, but this should work for now
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void DisplayText(FString toDisplay, UFCLockComponent* lock = nullptr);

	bool AdvanceText();
	void HandleText();
	void HandleTextToInventory(UFCLockComponent* lock);

	UInputComponent* inputComponent;
	AFCObjectWatcher* objectWatcher;
	AFCContainer* container;

	UFCInfoTextWidget* display = nullptr;

	AFCPlayerCamera* playerCamera;

	FString currentLevel;

	int32 interactKey;

	float transitionTime = 1.0f;
	float transitionTimer = 0.0f;

	bool fadeIn = false;
	bool fadeOut = false;

	//Maybe temporary, using this to test out displaying text from the inventory
	bool alreadyPaused = false;

	FName nextLevel;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UFCInfoTextWidget> infoWidget;
};



