// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FCSwitchComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchDelegate, bool, isOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchEndDelegate);


class ACameraActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCSwitchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCSwitchComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
	void PressSwitch(bool pauseAnimation = true, bool hidePlayer = false);
	void SetUpScene(bool pauseAnimation, bool hidePlayer);
	//UFUNCTION()
	//void PlayScene();
	UFUNCTION()
	void EndScene();

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FSwitchDelegate Switch;
	//Using this to make sure all attached objects can remain in their "on" state
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
		FSwitchStartDelegate SwitchOn;
	//Fires after a scene ends;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FSwitchEndDelegate SwitchEnd;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
		bool switchState = false;

	//If we move the camera to show an effect, we don't want to return input until that effect is finished
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	bool playScene = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	ACameraActor* sceneCamera;
	//might be able to remove this
	ACameraActor* playerCamera;
	//How long the scene lasts in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
		float sceneLength;
};
