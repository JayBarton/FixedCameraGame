#pragma once
#include "FCCustomButtonWidget.h"
#include "FCCustomSButton.h"

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

TSharedRef<SWidget> UFCCustomButtonWidget::RebuildWidget()
{
	MyButton = SNew(SCustomButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}
