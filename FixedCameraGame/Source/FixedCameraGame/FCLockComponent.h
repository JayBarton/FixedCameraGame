// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FCLockComponent.generated.h"

UENUM(BlueprintType)
enum class LockType : uint8 {
	KEY UMETA(DisplayName = "KEY"),
	PUZZLE UMETA(DisplayName = "PUZZLE"),
	ONE_WAY_LOCKED UMETA(DisplayName = "ONE WAY LOCKED"),
	ONE_WAY_UNLOCK UMETA(DisplayName = "ONE WAY UNLOCK"),
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLockDestroyedDelegate);



//Used to handle interactables that can be locked, such as doors
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIXEDCAMERAGAME_API UFCLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFCLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	void Open(int32 keyID);

	//The id that opens this lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
		FString description = "Door is locked";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
		FString unlockDescription = "Door unlocked";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock")
		LockType type;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FLockDestroyedDelegate Unlocked;

};
