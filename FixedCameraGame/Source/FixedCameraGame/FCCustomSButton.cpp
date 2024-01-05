#include "FCCustomSButton.h"

FReply SCustomButton::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && GetNavigationActionFromKey(InKeyEvent.GetKey()) == EUINavigationAction::Accept)
	{
		Press();

		if (PressMethod == EButtonPressMethod::ButtonPress)
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}
	else
	{
		Reply = SBorder::OnKeyDown(MyGeometry, InKeyEvent);
	}

	//return the constructed reply
	return Reply;
}

EUINavigationAction SCustomButton::GetNavigationActionFromKey(const FKey& InKey)
{
	if (InKey == EKeys::SpaceBar || InKey == EKeys::Virtual_Accept)
	{
		// By default, enter, space, and gamepad accept are all counted as accept
		return EUINavigationAction::Accept;
	}
	else if (InKey == EKeys::Escape || InKey == EKeys::Virtual_Back)
	{
		// By default, escape and gamepad back count as leaving current scope
		return EUINavigationAction::Back;
	}

	return EUINavigationAction::Invalid;
}
