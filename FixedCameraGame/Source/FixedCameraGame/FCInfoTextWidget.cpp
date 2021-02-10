// Fill out your copyright notice in the Description page of Project Settings.


#include "FCInfoTextWidget.h"
#include "Kismet/KismetStringLibrary.h"

UFCInfoTextWidget::UFCInfoTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
	delay = normalDelay;
	index = 0;
	segmentLength = 31;
}

void UFCInfoTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	//if (!IsTextFinished())
	if (!IsSegmentFinished())
	{
		currentTime += InDeltaTime;

		if (currentTime >= delay)
		{
			if (index < text.Len())
			{
				currentTime = 0;
				displayedText.Append(UKismetStringLibrary::GetCharacterArrayFromString(text)[index]);
				index++;
			}
		}
	}
}

bool UFCInfoTextWidget::IsTextFinished()
{
	if (index >= text.Len())
	{
		return true;
	}
	return false;
}

bool UFCInfoTextWidget::IsSegmentFinished()
{
	if (index == nextIndex)
	{
		return true;
	}
	return false;
}

void UFCInfoTextWidget::SetUpSegments()
{
	nextIndex = segmentLength;

	numberOfSegments = ceil(float(text.Len()) / segmentLength);
}

void UFCInfoTextWidget::GetNextSegment()
{
	delay = normalDelay;
	nextIndex += segmentLength;
	//UE_LOG(LogTemp, Warning, TEXT("next index %i, current %i"), nextIndex, index);
	if (nextIndex > text.Len())
	{
		nextIndex = text.Len();
	}
	else
	{
		while (text.Mid(nextIndex - 1, 1) != "")
		{
			UE_LOG(LogTemp, Warning, TEXT("next starts at %s"), *text.Mid(nextIndex, 1));
			nextIndex--;
			//UE_LOG(LogTemp, Warning, TEXT("while next %i"), nextIndex);

		}
	}
	displayedText = "";

}
