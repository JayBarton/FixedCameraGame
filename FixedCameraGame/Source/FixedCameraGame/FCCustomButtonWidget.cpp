#pragma once
#include "FCCustomButtonWidget.h"

UFCCustomButtonWidget::UFCCustomButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PressedStyle.SetNormal(WidgetStyle.Pressed);
	PressedStyle.SetNormalPadding(WidgetStyle.PressedPadding);
	PressedStyle.SetPressed(WidgetStyle.Pressed);
	PressedStyle.SetPressedPadding(WidgetStyle.PressedPadding);
	PressedStyle.SetHovered(WidgetStyle.Pressed);
	PressedStyle.SetDisabled(WidgetStyle.Pressed);
	PressedStyle.Normal.TintColor = FSlateColor(FLinearColor(0.286458f, 1.000000f, 0.286458f));
	PressedStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.286458f, 1.000000f, 0.286458f));
	PressedStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.286458f, 1.000000f, 0.286458f));

	WidgetStyle.SetHovered(WidgetStyle.Normal);
	WidgetStyle.SetPressed(WidgetStyle.Normal);
	WidgetStyle.SetPressedPadding(0.0f);
	WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.286458f, 1.000000f, 0.286458f));
	DefaultStyle = WidgetStyle;

	FocusedStyle = DefaultStyle;
	FocusedStyle.Normal.TintColor = FSlateColor(FLinearColor(0.286458f, 1.000000f, 0.286458f));
	FocusedStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.286458f, 1.000000f, 0.286458f));
}

void UFCCustomButtonWidget::FalsePress()
{
	MyButton->SetButtonStyle(&PressedStyle);
	FTimerHandle ResetStyleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ResetStyleTimerHandle, this, &UFCCustomButtonWidget::ResetPress, 0.15f, false);
}

void UFCCustomButtonWidget::ResetPress()
{
	if (MyButton)
	{
		if (MyButton->HasKeyboardFocus())
		{
			MyButton->SetButtonStyle(&FocusedStyle);
		}
		else
		{
			MyButton->SetButtonStyle(&DefaultStyle);
		}
	}
}
