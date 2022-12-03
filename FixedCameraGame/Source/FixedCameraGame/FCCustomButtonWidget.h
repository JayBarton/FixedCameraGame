#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Input/Reply.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Components/ContentWidget.h"

#include "Components/Button.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"


#include "FCCustomButtonWidget.generated.h"

class SButton;
class USlateWidgetStyleAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonClickedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonPressedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonReleasedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonHoverEvent);

UCLASS()
class FIXEDCAMERAGAME_API UFCCustomButtonWidget : public UContentWidget
{
	GENERATED_BODY()

public:
	/** The template style asset, used to seed the mutable instance of the style. */
	UPROPERTY()
	USlateWidgetStyleAsset* Style_DEPRECATED;

	/** The button style used at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (DisplayName = "Style"))
	FButtonStyle WidgetStyle;

	/** The color multiplier for the button content */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
	FLinearColor ColorAndOpacity;

	/** The color multiplier for the button background */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
	FLinearColor BackgroundColor;

	/** The type of mouse action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** The type of touch action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** The type of keyboard/gamepad button press action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Sometimes a button should only be mouse-clickable and never keyboard focusable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool IsFocusable;

	/** Called when the button is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnCustomButtonClickedEvent OnClicked;

	/** Called when the button is pressed */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnCustomButtonPressedEvent OnPressed;

	/** Called when the button is released */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnCustomButtonReleasedEvent OnReleased;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnCustomButtonHoverEvent OnHovered;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnCustomButtonHoverEvent OnUnhovered;

public:
	TSharedPtr<SButton> MyButton;

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override
	{
		Super::ReleaseSlateResources(bReleaseChildren);

		MyButton.Reset();
	}
	//~ End UVisual Interface

	//~ Begin UObject Interface
	virtual void PostLoad() override
	{
		Super::PostLoad();

	}
	//~ End UObject Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override
	{
		return FText::FromString("Custom Widgets");
	}
#endif
protected:
protected:
	/** Handle the actual click event from slate and forward it on */
	FReply SlateHandleClicked();
	void SlateHandlePressed();
	void SlateHandleReleased();
	void SlateHandleHovered();
	void SlateHandleUnhovered();

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override { return Content; }
#endif
	//~ End UWidget Interface

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override
	{
		return MyButton;

	}
#endif

	virtual UClass* GetSlotClass() const override
	{
		return UButtonSlot::StaticClass();
	}
	virtual void OnSlotAdded(UPanelSlot* InSlot) override
	{
		// Add the child to the live slot if it already exists
		if (MyButton.IsValid())
		{
			CastChecked<UButtonSlot>(InSlot)->BuildSlot(MyButton.ToSharedRef());
		}
	}
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override
	{
		// Remove the widget from the live slot if it exists.
	/*	if (MyButton.IsValid())
		{
			MyButton->SetContent(SNullWidget::NullWidget);
		}*/
	}
};