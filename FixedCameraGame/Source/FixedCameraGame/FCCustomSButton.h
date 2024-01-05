#pragma once
#include "CoreMinimal.h"
#include "Widgets/Input/SButton.h"
#include "Input/Reply.h"
class SCustomButton
	: public SButton
{
public:

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	EUINavigationAction GetNavigationActionFromKey(const FKey& InKeyEvents);
};