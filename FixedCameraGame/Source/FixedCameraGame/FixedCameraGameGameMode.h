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
class AFCInteractable;
class UFCSwitchComponent;

//Definitely temporary
class USoundBase;
class USoundWave;
class UAudioComponent;

DECLARE_DELEGATE_OneParam(InputLockDelegate, UFCLockComponent*);
DECLARE_DELEGATE_OneParam(InputInteractDelegate, AFCInteractable*);
DECLARE_DELEGATE_OneParam(InputSceneDelegate, UFCSwitchComponent*);


UCLASS(minimalapi)
class AFixedCameraGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFixedCameraGameGameMode();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void CheckInstance(UFCGameInstance* instance);

	void CheckObjects(UFCGameInstance* instance);

	void CheckEnemies(UFCGameInstance* instance);

	void SpawnEnemies(UFCGameInstance* instance, bool firstTime = false);

	void HandlePendingLocks(UFCGameInstance* instance);

	void FindStart(UFCGameInstance* instance);

	void ChangeLevel(int index, int cameraIndex, FName levelName);
	void MoveToLevel(FName levelName);
	void ResetLevel();

	void SetPendingLock(FString levelName, int32 index);

	void StopEnemies(bool pauseAnimation = true);
	void ResumeEnemies();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//I'm hoping I can find a better way to handle the locks, but this should work for now
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void DisplayText(FString toDisplay, UFCLockComponent* lock = nullptr, AFCInteractable* interactable = nullptr, UFCSwitchComponent* Switch = nullptr, bool advanceClear = true);

	bool AdvanceText();
	void HandleText();
	void HandleTextToInteractable(AFCInteractable* interactable);
	void HandleTextToInventory(UFCLockComponent* lock);
	void HandleTextToScene(UFCSwitchComponent* Switch);


	UFUNCTION(BlueprintCallable, Category = "Functions")
	void ClearText();

	UInputComponent* inputComponent;
	AFCObjectWatcher* objectWatcher;
	AFCContainer* container;

	UPROPERTY(BlueprintReadOnly, Category = "Text")
	UFCInfoTextWidget* display = nullptr;

	AFCPlayerCamera* playerCamera;

	FString currentLevel;

	int32 currentIndex;

	int32 currentCameraIndex;

	int32 interactKey;

	float transitionTime = 1.0f;
	float transitionTimer = 0.0f;

	bool levelFadeOut = false;
	bool levelFadeIn = false;

	bool newLevel = true;

	//Maybe temporary, using this to test out displaying text from the inventory
	bool inMenu = false;

	FName nextLevel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UFCInfoTextWidget> infoWidget;


	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* tempMusic;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UAudioComponent* music = nullptr;

};



