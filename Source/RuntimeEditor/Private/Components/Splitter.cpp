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


#include "Components/Splitter.h"
#include "Components/SplitterSlot.h"

#include "Widgets/Layout/SSplitter.h"

static FSplitterStyle* DefaultSplitterStyle = nullptr;

USplitter::USplitter()
{
	bIsVariable = false;
	Visibility = ESlateVisibility::Visible;

	Orientation = EOrientation::Orient_Horizontal;
	ResizeMode = ERealSplitterResizeMode::FixedPosition;
	PhysicalSplitterHandleSize = 5.0f;
	HitDetectionSplitterHandleSize = 5.0f;
	MinimumSlotHeight = 20.0f;

	if (DefaultSplitterStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultSplitterStyle = new FSplitterStyle(FCoreStyle::Get().GetWidgetStyle<FSplitterStyle>("Splitter"));

		// Unlink UMG default colors from the editor settings colors.
		//DefaultSplitterStyle->UnlinkColors();
		DefaultSplitterStyle->HandleNormalBrush.UnlinkColors();
		DefaultSplitterStyle->HandleHighlightBrush.UnlinkColors();
	}

	WidgetStyle = *DefaultSplitterStyle;
}



UClass* USplitter::GetSlotClass() const
{
	return USplitterSlot::StaticClass();
}

void USplitter::OnSlotAdded(UPanelSlot* InSlot)
{
	//Add the child to the live canvas if it already exists
	if (MySplitter.IsValid())
	{
		CastChecked<USplitterSlot>(InSlot)->BuildSlot(MySplitter.ToSharedRef());
	}
}

void USplitter::OnSlotRemoved(UPanelSlot* InSlot)
{
	//Remove the widget from the live slot if it exists
	if (MySplitter.IsValid() && InSlot->Content)
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			//MySplitter->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

void USplitter::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySplitter.Reset();
}

TSharedRef<SWidget> USplitter::RebuildWidget()
{

	MySplitter = SNew(SSplitter)
		.Style(&WidgetStyle)
		.Orientation(Orientation)
		.ResizeMode(static_cast<ESplitterResizeMode::Type>(ResizeMode.GetValue()))
		.PhysicalSplitterHandleSize(PhysicalSplitterHandleSize)
		.HitDetectionSplitterHandleSize(HitDetectionSplitterHandleSize)
		.MinimumSlotHeight(MinimumSlotHeight)
		.OnSplitterFinishedResizing(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleSplitterFinishedResizing));

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (USplitterSlot* TypedSlot = Cast<USplitterSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MySplitter.ToSharedRef());
		}
	}

	return MySplitter.ToSharedRef();
}

void USplitter::HandleSplitterFinishedResizing()
{
	OnSplitterFinishedResizing.Broadcast();
}
