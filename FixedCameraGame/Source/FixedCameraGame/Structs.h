#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "Structs.generated.h"


USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int ID = -1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int maxCapacity = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int amount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int healAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int combineWith;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int combineResult;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FString description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UTexture2D* thumbnail;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool isStackable = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool isConsumable = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool isCombinable = false;
};