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
class AFCPuzzleInteractable;

//Definitely temporary
class USoundBase;
class USoundWave;
class UAudioComponent;

DECLARE_DELEGATE_OneParam(InputLockDelegate, UFCLockComponent*);
DECLARE_DELEGATE_OneParam(InputInteractDelegate, AFCInteractable*);
DECLARE_DELEGATE_TwoParams(InputSceneDelegate, UFCSwitchComponent*, AFCInteractable*);


UCLASS(minimalapi)
class AFixedCameraGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFixedCameraGameGameMode();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void CheckInstance(UFCGameInstance* instance);

	void HandleMusic(UFCGameInstance* instance);

	//Not sure about this function, but it should work for now
	void PlayMusic(UFCGameInstance* instance);

	//Called outside of BeginPlay
	void StartNewMusic(bool playOnce = false);

	UFUNCTION(BlueprintCallable)
	void StopMusic();

	void CheckObjects(UFCGameInstance* instance);

	void CheckEnemies(UFCGameInstance* instance);

	void SpawnEnemies(UFCGameInstance* instance, bool firstTime = false);

	void HandlePendingLocks(UFCGameInstance* instance);

	void FindStart(UFCGameInstance* instance);

	void ChangeLevel(int index, int cameraIndex, FName levelName, bool continueMusic);
	void MoveToLevel(FName levelName);
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void EndGame(bool dead = true);

	void SetPendingLock(FString levelName, int32 index);

	void StopEnemies(bool pauseAnimation = true, bool hideEnemy = false);
	void ResumeEnemies();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void SaveGame(int slot, int token, int playerStart);
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void LoadGame(int slot);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//I'm hoping I can find a better way to handle the locks, but this should work for now
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void DisplayText(FString toDisplay, UFCLockComponent* lock = nullptr, AFCInteractable* interactable = nullptr, UFCSwitchComponent* Switch = nullptr, bool advanceClear = true, AFCPlayerCamera* playerCamera = nullptr, UFCInfoTextWidget* displayWidget = nullptr);

	bool AdvanceText();
	void HandleText();
	void HandleTextToInteractable(AFCInteractable* interactable);
	void HandleTextToInventory(UFCLockComponent* lock);
	void HandleTextToScene(UFCSwitchComponent* Switch, AFCInteractable* interactable);
	void HandleTextFromCamera();

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void ClearText();

	void LevelChangeSound(int32 index);

	//if there is a blue print version of FName::NameToDisplayString I can't find it
	UFUNCTION(BlueprintCallable, Category = "Functions")
	FString GetDisplayName(FString name);

	UFUNCTION(BlueprintCallable, Category = "Functions")
	FString GetTime();

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

	int32 enemyKills;

	float transitionTime = 1.0f;
	float transitionTimer = 0.0f;

	float gameTime = 0.0f;

	bool levelFadeOut = false;
	bool levelFadeIn = false;

	bool newLevel = true;

	UPROPERTY(BlueprintReadOnly, Category = "Text")
	bool displayingText = false;
	bool inMenu = false;

	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	bool fakePause = false;

	FName nextLevel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UFCInfoTextWidget> infoWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* tempMusic;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UAudioComponent* music = nullptr;

	//Sound that plays on entering level
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TArray<USoundBase*> enterSound;

	//Sound that plays on exiting level
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TArray<USoundBase*> exitSound;
};



