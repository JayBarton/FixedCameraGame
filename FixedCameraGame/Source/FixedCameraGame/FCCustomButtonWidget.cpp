#pragma once
#include "FCCustomButtonWidget.h"

void UFCCustomButtonWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
//	MyButton->SetColorAndOpacity(ColorAndOpacity);
//	MyButton->SetBorderBackgroundColor(BackgroundColor);
}

TSharedRef<SWidget> UFCCustomButtonWidget::RebuildWidget()
{
	MyButton = SNew(SButton)
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

FReply UFCCustomButtonWidget::SlateHandleClicked()
{
	OnClicked.Broadcast();

	return FReply::Handled();
}

void UFCCustomButtonWidget::SlateHandlePressed()
{
	OnPressed.Broadcast();
}

void UFCCustomButtonWidget::SlateHandleReleased()
{
	OnReleased.Broadcast();
}

void UFCCustomButtonWidget::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void UFCCustomButtonWidget::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}