/*
The MIT License (MIT)

Copyright (c) 2022 TT

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelSlot.h"
#include "Widgets/Layout/SSplitter.h"
#include "SplitterSlot.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal(float, FOnGetSizeValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplitterSlotResized, float, NewSize /*The new size coefficient of the slot*/);

// /** How should a child's size be determined */
// enum ESizeRule
// {
// 	/** Get the DesiredSize() of the content */
// 	SizeToContent,
// 	/** Use a fraction of the parent's size */
// 	FractionOfParent
// };

UENUM(BlueprintType)
enum class ERealSizeRule:uint8
{
	/** Get the DesiredSize() of the content */
	SizeToContent,
	/** Use a fraction of the parent's size */
	FractionOfParent
};

USTRUCT(BlueprintType, BlueprintAble)
struct FColumnSizeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FOnGetSizeValue OnGetSlotSizeValue;
};

UCLASS(BlueprintType, Blueprintable)
class RUNTIMEEDITOR_API USplitterSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

public:

	USplitterSlot();

	/** How much space this slot should occupy in the direction of the panel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Splitter Slot", meta = (DisplayAfter = "Padding"))
	float SizeValue;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Splitter Slot", meta = (DisplayAfter = "Padding"))
	ERealSizeRule SizeRule;

	/** Called when the menu content is requested to allow a more customized handling over what to display */
	UPROPERTY(EditAnywhere, Category = "Layout|Splitter Slot", meta = (IsBindableEvent = "True"))
	FOnGetSizeValue OnGetSlotSizeValue;

	UFUNCTION(BlueprintCallable)
	void SetSlotGetSizeEvent(FOnGetSizeValue InEvent) { OnGetSlotSizeValue = InEvent; }

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnSplitterSlotResized OnSplitterSlotResized;

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/** Builds the underlying FSlot for the Slate layout panel. */
	void BuildSlot(TSharedRef<SSplitter> InSplitter);

protected:
	void HandleSlotResized(float NewSize);
	float HandleGetSlotValue();

private:
	SSplitter::FSlot* Slot;
};
