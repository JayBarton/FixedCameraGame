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

UCLASS()
class FIXEDCAMERAGAME_API UFCCustomButtonWidget : public UButton
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Functions")
	void FalsePress();

	void ResetPress();

	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, Category = "Style")
	FButtonStyle DefaultStyle;
	UPROPERTY(BlueprintReadWrite, Category = "Style")
	FButtonStyle PressedStyle;
	UPROPERTY(BlueprintReadWrite, Category = "Style")
	FButtonStyle FocusedStyle;
};