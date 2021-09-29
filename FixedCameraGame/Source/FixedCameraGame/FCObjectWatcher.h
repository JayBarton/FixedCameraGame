// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCObjectWatcher.generated.h"


class AFCEnemy;
class AFCEnemySpawn;

//Struct of the data to keep track of
USTRUCT(BlueprintType)
struct FDataToWatch
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        AActor* actor;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        bool spawn = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        bool locked = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        bool active = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        bool switched = false;
};

//Struct of the enemies to keep track of
USTRUCT(BlueprintType)
struct FEnemiesToWatch
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Data")
    AFCEnemySpawn* spawnActor;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
    AFCEnemy* enemy;
    //if this enemy is in the scene, alive or dead
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    bool spawn = true;
    //if this enemy is spawnable. Dead enemies are not spawnable.
    bool canSpawn = true;
    //if this enemy has been killed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    bool alive = true;
    //If the enemy starts in their idle state or in a spawn in state
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    bool spawnIn = false;
    int32 reviveCount = 0;
    int32 reviveTime = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FString spawnFlag = "";
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
    FTransform transform;
};

//Have to create an array in this struct to get around ue4 not allowing double nesting
USTRUCT(BlueprintType)
struct FDataArray
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    TArray<FDataToWatch> data;
};

USTRUCT(BlueprintType)
struct FEnemyArray
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        TArray<FEnemiesToWatch> data;
};

USTRUCT(BlueprintType)
struct FPendingLocks
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        TArray<int32> data;
};

USTRUCT(BlueprintType)
struct FMusicPlaying
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        bool data;
};

//Use this class to keep track of objects after leaving a map
UCLASS()
class FIXEDCAMERAGAME_API AFCObjectWatcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCObjectWatcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    void UpdateObjects();

    void CheckEnemies();

    void CheckObjects();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        FDataArray objects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        FEnemyArray enemies;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags")
        TArray<FString> pendingFlags;

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundBase* levelMusic = nullptr;

    UPROPERTY(EditAnywhere, Category = "Audio")
        bool playingMusic = true;

};
