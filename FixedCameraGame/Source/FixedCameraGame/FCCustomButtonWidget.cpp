#pragma once
#include "FCCustomButtonWidget.h"

UFCCustomButtonWidget::UFCCustomButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WidgetStyle.SetHovered(WidgetStyle.Normal);
	DefaultStyle = WidgetStyle;

	PressedStyle.SetNormal(WidgetStyle.Pressed);
	PressedStyle.SetNormalPadding(WidgetStyle.PressedPadding);
	PressedStyle.SetPressed(WidgetStyle.Pressed);
	PressedStyle.SetPressedPadding(WidgetStyle.PressedPadding);
	PressedStyle.SetHovered(WidgetStyle.Pressed);
	PressedStyle.SetDisabled(WidgetStyle.Pressed);
}

void UFCCustomButtonWidget::FalsePress()
{
	MyButton->SetButtonStyle(&PressedStyle);
	FTimerHandle ResetStyleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ResetStyleTimerHandle, this, &UFCCustomButtonWidget::ResetPress, 0.15f, false);
}

void UFCCustomButtonWidget::ResetPress()
{
	MyButton->SetButtonStyle(&DefaultStyle);
}
