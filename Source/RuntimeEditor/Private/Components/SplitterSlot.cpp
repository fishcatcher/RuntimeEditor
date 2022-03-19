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


#include "Components/SplitterSlot.h"
#include "Components/Widget.h"

USplitterSlot::USplitterSlot() :
	Slot(nullptr)
{
	SizeRule = ERealSizeRule::FractionOfParent;
	SizeValue = 1;
}

USplitterSlot::USplitterSlot(const FObjectInitializer&) :
	USplitterSlot()
{

}

void USplitterSlot::SynchronizeProperties()
{
	if (Slot)
	{
		Slot->SizeValue = SizeValue;
		Slot->SizingRule = static_cast<SSplitter::ESizeRule>(SizeRule);
	}
}

void USplitterSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slot = nullptr;
}

void USplitterSlot::BuildSlot(TSharedRef<SSplitter> InSplitter)
{
	Slot = &InSplitter->AddSlot()
		.Value(BIND_UOBJECT_ATTRIBUTE(float, HandleGetSlotValue))
		.SizeRule(static_cast<SSplitter::ESizeRule>(SizeRule))
		.OnSlotResized(BIND_UOBJECT_DELEGATE(SSplitter::FOnSlotResized, HandleSlotResized))
		[
			Content == NULL ? SNullWidget::NullWidget : Content->TakeWidget()
		];
}

void USplitterSlot::HandleSlotResized(float NewSize)
{
	OnSplitterSlotResized.Broadcast(NewSize);
	SizeValue = NewSize;
}

float USplitterSlot::HandleGetSlotValue()
{
	if (OnGetSlotSizeValue.IsBound())
	{
		return OnGetSlotSizeValue.Execute();
	}
	return SizeValue;
}

