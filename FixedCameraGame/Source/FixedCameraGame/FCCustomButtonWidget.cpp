#pragma once
#include "FCCustomButtonWidget.h"

static FButtonStyle* DefaultButtonStyle = nullptr;
static FButtonStyle PressedStyle;

UFCCustomButtonWidget::UFCCustomButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultButtonStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultButtonStyle->UnlinkColors();
	}

	PressedStyle.Normal = FSlateBrush();
	PressedStyle.Normal.TintColor = DefaultButtonStyle->Pressed.TintColor;
	PressedStyle.NormalPadding = DefaultButtonStyle->NormalPadding;
	PressedStyle.Hovered = FSlateBrush();
	PressedStyle.Hovered.TintColor = DefaultButtonStyle->Pressed.TintColor;
	PressedStyle.Pressed = FSlateBrush();
	PressedStyle.Pressed.TintColor = DefaultButtonStyle->Pressed.TintColor;
	PressedStyle.PressedPadding = DefaultButtonStyle->PressedPadding;
	//PressedStyle.SetNormal (DefaultButtonStyle->Hovered);
	//PressedStyle.SetHovered (DefaultButtonStyle->Hovered);
	//PressedStyle.SetPressed(DefaultButtonStyle->Hovered);

	DefaultButtonStyle->SetHovered(DefaultButtonStyle->Normal);
	DefaultButtonStyle->SetPressed(DefaultButtonStyle->Normal);

	WidgetStyle = *DefaultButtonStyle;


	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::DownAndUp;
	TouchMethod = EButtonTouchMethod::DownAndUp;

	IsFocusable = true;

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

void UFCCustomButtonWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MyButton->SetColorAndOpacity(ColorAndOpacity);
	MyButton->SetBorderBackgroundColor(BackgroundColor);
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
//	OnClicked.Broadcast();
	MyButton->SetButtonStyle(&PressedStyle);
	//MyButton->Invalidate(EInvalidateWidgetReason::None);

	//FPlatformProcess::Sleep(0.5f);
	//MyButton->SetButtonStyle(DefaultButtonStyle);
	//MyButton->Invalidate(EInvalidateWidgetReason::None);
	///WidgetStyle = PressedStyle;

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