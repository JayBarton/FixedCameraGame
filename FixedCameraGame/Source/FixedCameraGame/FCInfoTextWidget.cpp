// Fill out your copyright notice in the Description page of Project Settings.


#include "FCInfoTextWidget.h"
#include "Kismet/KismetStringLibrary.h"

UFCInfoTextWidget::UFCInfoTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
	delay = normalDelay;
	index = 0;
}

void UFCInfoTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	if (!IsTextFinished())
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
	if (index == text.Len())
	{
		return true;
	}
	return false;
}
