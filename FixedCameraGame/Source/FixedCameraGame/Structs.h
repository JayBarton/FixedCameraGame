#pragma once

#include "CoreMinimal.h"
#include "Structs.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int ID = -1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FString description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UTexture2D* thumbnail;
};