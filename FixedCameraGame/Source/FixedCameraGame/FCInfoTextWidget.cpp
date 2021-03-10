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
	FindNextIndex(0);
}

void UFCInfoTextWidget::GetNextSegment()
{
	delay = normalDelay;
	int startingIndex = nextIndex;
	nextIndex += segmentLength;
	FindNextIndex(startingIndex);

	displayedText = "";
}

void UFCInfoTextWidget::FindNextIndex(int startingIndex)
{
	if (nextIndex >= text.Len())
	{
		nextIndex = text.Len();
	}
	else
	{
		//Don't want to cut off part of a word, so if the segment ends part way through a word,
		//move back until we find a space
		while (text.Mid(nextIndex - 1, 1) != " ")
		{
			nextIndex--;
		}
		//Using ! as an escape key. If we find one in the string, we end the segment there.
		for (int i = startingIndex; i < nextIndex; i++)
		{
			if ((text.Mid(i, 1) == "!"))
			{
				nextIndex = i;
				text.RemoveAt(i, 1);
				break;
			}
		}
	}
}
