// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCObjectWatcher.generated.h"


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
struct FPendingLocks
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        TArray<int32> data;
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
        FDataArray objects;

};
