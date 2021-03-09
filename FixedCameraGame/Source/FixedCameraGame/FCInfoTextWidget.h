// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FCInfoTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIXEDCAMERAGAME_API UFCInfoTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFCInfoTextWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
		bool IsTextFinished();

	bool IsSegmentFinished();

	void SetUpSegments();
	void GetNextSegment();

	void FindNextIndex(int startingIndex);

	//Using this so that I can still advance the text from the pickup prompt, but not close the widget until a button is press
	//Real brute force solution to this problem
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	bool advanceClear = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FString text;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FString displayedText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		int32 index;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		int32 segmentLength;
	int32 nextIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float currentTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float delay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		float normalDelay = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		float fastDelay = 0.0025f;
};
