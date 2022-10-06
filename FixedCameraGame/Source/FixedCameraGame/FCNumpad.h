#pragma once

#include "CoreMinimal.h"
#include "FCPuzzleInteractable.h"
#include "FCNumpad.generated.h"

/**
 *
 */

class UWidgetComponent;
class ACameraActor;

UCLASS()
class FIXEDCAMERAGAME_API AFCNumpad : public AFCPuzzleInteractable
{
	GENERATED_BODY()

public:
	AFCNumpad();

	virtual void BeginPlay() override;

	virtual void StartPuzzle();

	//Sends the password to the widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
		void SendToWidget();

	virtual void OpenLock() override;
	virtual void ExitPuzzle() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pad")
		FString password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pad")
		UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
		ACameraActor* newCamera;

};
