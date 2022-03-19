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
#include "Components/PanelWidget.h"
#include "Styling/SlateTypes.h"
#include "Splitter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSplitterFinishedResizingEvent);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FVector2D, FOnSplitterGetMaxSlotSizeEvent, int32, SlotIndex);

/*
namespace ESplitterResizeMode
{
	enum Type
	{
		FixedPosition,
		FixedSize,
		Fill,
	};
}
*/

/** An enum'ed type of ESplitterResizeMode */
UENUM(BlueprintType)
namespace ERealSplitterResizeMode
{
	enum Type
	{
		FixedPosition,
		FixedSize,
		Fill,
	};
}

/**
 * Wrapper class for SSplitter
 */
UCLASS()
class RUNTIMEEDITOR_API USplitter : public UPanelWidget
{
	GENERATED_BODY()

public:
	USplitter();

	/** Style of the splitter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Style"))
		FSplitterStyle WidgetStyle;

	/**  */
	UPROPERTY(EditAnywhere, Category = "Behavior")
		float PhysicalSplitterHandleSize;

	/**  */
	UPROPERTY(EditAnywhere, Category = "Behavior")
		float HitDetectionSplitterHandleSize;

	/**  */
	UPROPERTY(EditAnywhere, Category = "Behavior")
		float MinimumSlotHeight;

	/**  */
	UPROPERTY(EditAnywhere, Category = "Behavior")
		TEnumAsByte<ERealSplitterResizeMode::Type> ResizeMode;

	/**  */
	UPROPERTY(EditAnywhere, Category = "Behavior")
		TEnumAsByte<EOrientation> Orientation;

	FOnSplitterFinishedResizingEvent OnSplitterFinishedResizing;
	FOnSplitterGetMaxSlotSizeEvent OnSplitterGetMaxSlotSize;

protected:
	void HandleSplitterFinishedResizing();

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	// End UPanelWidget
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	TSharedPtr<class SSplitter> MySplitter;

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
